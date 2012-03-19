// DEBUG
#include <iostream>
#include <vector>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Sparse>
#include <eigen3/Eigen/IterativeLinearSolvers>
#include "FluidSolver.h"
#include "IFluidRenderer.h"
#include "Grid.h"
#include "Cell.h"
#include "Vector2.h"
#include "SignalRelay.h"


using std::vector;
using Eigen::VectorXd;
using Eigen::VectorXi;
using Eigen::SparseMatrix;
using Eigen::Triplet;
using Eigen::RowMajor;
using Eigen::ConjugateGradient;
using Eigen::Success;
typedef Triplet<double> Tripletd;

FluidSolver::FluidSolver(float width, float height)
  : _width(width),
    _height(height),
    _grid(_width, _height),
    _frameReady(false),
    _particles()
{
  // Provide default values to the grid.
  reset();

  // Connect ourselves to the 'resetSimulation' signal.
  QObject::connect(SignalRelay::getInstance(), SIGNAL(resetSimulation()),
		   this, SLOT(reset()));
}


FluidSolver::~FluidSolver()
{
  // Disconnect from receiving any signals.
  SignalRelay::getInstance()->disconnect(this);
}


void FluidSolver::reset()
{
  // Note: This is super ghetto currently, and simply initializes the solver
  // with a large default grid, all filled with fluid and arbitrary velocities.
  // Change this as needed during devlopment to quickly test stuff out, and
  // remove before final release!

  // Delete existing particles
  _particles.clear();
  
  // Initialize a velocity field for testing.
  // Note: values in this velocity field are arbitrarily chosen and may be
  //  divergent within a cell.
  // Note: sin() is used to clamp output values to [-1, 1].
  Grid grid(_width, _height);
  for (unsigned y = _height / 2; y < _height; ++y) 
    for (unsigned x = _width / 2; x < _width; ++x) {
      grid(x,y).cellType = Cell::FLUID;
      grid(x,y).pressure = 0.0f;

      // Initialize marker particle positions.
      for (unsigned i = 0; i < 4; i++)
        for(unsigned j = 0; j < 4; j++) 
	  _particles.push_back(Vector2(x + 0.20f * (i + 1), y + 0.20f * (j + 1)));
  }

  // Set values accordingly.
  _grid = grid;
  _frameReady = false;
}

void FluidSolver::advanceFrame()
{
  float frameTimeSec = 1.0f/30.0f; // TODO Target 30 Hz framerate for now.
  float CFLCoefficient = 2.0f;     // TODO CFL coefficient set to 2 for now.

  while (!_frameReady) {
    // If enough simulation time has elapsed to draw the next frame, break.
    if (frameTimeSec <= 0.0f) {
      _frameReady = true;
      break;
    }

    // Calculate an appropriate timestep based on the estimated max velocity
    // and the CFL coefficient.
    float simTimeStepSec = CFLCoefficient / _grid.getMaxVelocity().magnitude();
    
    // If the remaining time to simulate in this frame is less than the
    // CFL-calculated time, just advance the sim for the remaining frame time.
    if (simTimeStepSec > frameTimeSec) {
      simTimeStepSec = frameTimeSec;
    }
    
    // Advance the simulation by the calculated timestep.  Decrement the
    // "remaining" time for this frame.
    advanceTimeStep(simTimeStepSec);
    frameTimeSec -= simTimeStepSec;
  }
}


void FluidSolver::advanceTimeStep(float timeStepSec)
{
  Vector2 gravity(0.0f, -9.8f);  // Gravity: -0.098 cells/sec^2

  advectVelocity(timeStepSec);
  applyGlobalVelocity(gravity * timeStepSec);
  boundaryCollide();
  pressureSolve(timeStepSec);
  boundaryCollide();
  moveParticles(timeStepSec);
  markCells();
}


void FluidSolver::advectVelocity(float timeStepSec)
{
  for(float x = 0; x < _grid.getWidth(); x += 1.0f)
    for( float y = 0.5; y < _grid.getHeight(); y += 1.0f) {
      Cell &cell = _grid(floor(x), floor(y));
      Vector2 position(x, y); 
      position = particleTrace(position, timeStepSec);
      cell.stagedVel[Cell::X] = _grid.getVelocity(position).x;
    }
  for(float y = 0; y < _grid.getHeight(); y += 1.0f)
    for(float x = 0.5; x < _grid.getWidth(); x+= 1.0f) {
      Cell &cell = _grid(floor(x), floor(y));
      Vector2 position(x, y);
      position = particleTrace(position, timeStepSec);
      cell.stagedVel[Cell::Y] = _grid.getVelocity(position).y;
    }
  for(unsigned i = 0; i < _grid.getRowCount() * _grid.getColCount(); i++) {
    _grid[i].commitStagedVel();
  }
}

// This function only enforces boundary condtitions at the grid borders,
// not on the free surface
Vector2 FluidSolver::particleTrace(Vector2 position, float timeStepSec)
{
  Vector2 velocity = _grid.getVelocity(position) * -timeStepSec;
  Vector2 toPosition = position + velocity;
  Vector2 tempPos;
  float width = _grid.getWidth();
  float height = _grid.getHeight();
  float dist;
  float interceptX = 0.0f;
  float interceptY = 0.0f; 
  bool intersectX = toPosition.x < 0 || toPosition.x > _grid.getWidth(); 
  bool intersectY = toPosition.y < 0 || toPosition.y > _grid.getHeight(); 

  if (intersectX || intersectY) {
    if (velocity.x > 0)
      interceptX = width;
    if (velocity.y > 0)
      interceptY = height;
    dist = interceptX - position.x / velocity.x;
    tempPos.y = position.y + dist * velocity.y;
    tempPos.x = interceptX;
    dist = interceptY - position.y / velocity.y;
    position.x += dist * velocity.y;
    if (tempPos.magnitude() < position.magnitude())
      tempPos = position;
  }
  else
    position = toPosition;
  return position;
}

void FluidSolver::applyGlobalVelocity(Vector2 velocity)
{
  // Apply the provided velocity to all cells in the simulation.
  for (unsigned y = 0; y < _grid.getHeight(); ++y) 
    for (unsigned x = 0; x < _grid.getWidth(); ++x) {
      Cell &cell = _grid(x,y);
      if (cell.cellType == Cell::FLUID) {
	cell.vel[Cell::X] += velocity.x;
	cell.vel[Cell::Y] += velocity.y;
      }
    }
}


void FluidSolver::pressureSolve(float timeStepSec)
{
  // NOTE: assumptions are made here that the only "SOLID" cells in the sim
  // are the walls of the simulation, and are therefore not handled in this
  // method.  Instead, they are handled in the boundaryCollide method where
  // velocities entering or exiting a boundary are simply set to 0.0f.

  // The pressureSolve routine does the following:
  // * Calculate the negative divergence b with moditications at solid walls.
  // * Set the entries of A (stored in Adiag, etc.)
  // * Solve the Ap = b using the conjugate gradient algorithm.
  // * Compute the new velocities according to the updated pressure.

  // Calculate the dimensionality of our vectors/matrix.
  const unsigned width  = _grid.getColCount() - 1;
  const unsigned height = _grid.getRowCount() - 1;
  int dim = width * height;

  // Calculate the negative divergence throughout the simulation.
  VectorXd b(dim);
  for (unsigned y = 0; y < height; ++y)
    for (unsigned x = 0; x < width; ++x) {
      unsigned index = y * width + x;
      b(index) = -_grid.getVelocityDivergence(x, y);
    }

  // Update the negative divergence to account for solid boundaries.
  // TODO assumes that the only solids are at the boundaries, which have 0 vel.
  // Bottom row.
  for (unsigned x = 0; x < width; ++x) {
    unsigned y = 0;
    unsigned index = y * width + x;
    b(index) -= _grid(x,y).vel[Cell::Y];
  }
  // Top row.
  for (unsigned x = 0; x < width; ++x) {
    unsigned y = height - 1;
    unsigned index = y * width + x;
    b(index) += _grid(x,y+1).vel[Cell::Y];
  }
  // Left column.
  for (unsigned y = 0; y < height; ++y) {
    unsigned x = 0;
    unsigned index = y * width + x;
    b(index) -= _grid(x,y).vel[Cell::X];
  }
  // Right column.
  for (unsigned y = 0; y < height; ++y) {
    unsigned x = width - 1;
    unsigned index = y * width + x;
    b(index) += _grid(x+1,y).vel[Cell::X];
  }

  // Enforce the compatibility condition.
  // TODO don't need this if we have a free surface.
  /*
  double mean = 0.0;
  for (unsigned y = 0; y < height; ++y)
    for (unsigned x = 0; x < width; ++x) {
      unsigned index = y * width + x;
      mean += b(index);
    }
  //  std::cout.precision(15);
  //  std::cout << "Overall Divergence: " << mean << std::endl;
  mean /= dim;
  //  std::cout << "PerEntry Divergence: " << mean << std::endl;
  //  std::cout << "Pre-compat Divergence: " << std::endl << b << std::endl;
  for (unsigned y = 0; y < height; ++y)
    for (unsigned x = 0; x < width; ++x) {
      unsigned index = y * width + x;
      b(index) -= mean;
    }
  //  std::cout << "Post-compat Divergence: " << std::endl << b << std::endl;
  */

  // Set the entries of A. 
  std::vector< Tripletd > vals;
  SparseMatrix<double,RowMajor> A(dim, dim);
  for (unsigned y = 0; y < height; ++y)
    for (unsigned x = 0; x < width; ++x) {
      Cell &cell = _grid(x,y);
      unsigned i = y * width + x;  // this cell's col/row in A.
      unsigned j;                  // neighbor cell's col/row in A

      switch (cell.cellType) {
      case (Cell::SOLID):
	// If this cell is a SOLID, leave the entire row as 0's.
	break;

      case (Cell::AIR):
	// If this cell is AIR, increment neighboring fluid diagonals' coeff.
	if (cell.neighbors[Cell::POS_X]->cellType == Cell::FLUID) {
	  j = y * width + x + 1;                        // rt neighbor's idx
	  vals.push_back( Tripletd(j,j,timeStepSec) );  // rt neighbor's diag
	}
	if (cell.neighbors[Cell::POS_Y]->cellType == Cell::FLUID) {
	  j = (y + 1) * width + x;                      // up neighbor's idx
	  vals.push_back( Tripletd(j,j,timeStepSec) );  // up neighbor's diag
	}
	break;

      case (Cell::FLUID):
	// Cell is fluid. Determine coefficients of self and neighbors.
	if (cell.neighbors[Cell::POS_X]->cellType == Cell::FLUID) {
	  j = y * width + x + 1;                        // rt neighbor's idx
	  vals.push_back( Tripletd(i,i,timeStepSec) );  // my diagonal coeff
	  vals.push_back( Tripletd(i,j,-timeStepSec) ); // rt neighbor's coeff
	  vals.push_back( Tripletd(j,j,timeStepSec) );  // rt neighbor's diag
	}
	else if (cell.neighbors[Cell::POS_X]->cellType == Cell::AIR) {
	  vals.push_back( Tripletd(i,i,timeStepSec) );
	}
	if (cell.neighbors[Cell::POS_Y]->cellType == Cell::FLUID) {
	  j = (y + 1) * width + x;                      // up neighbor's idx
	  vals.push_back( Tripletd(i,i,timeStepSec) );  // my diagonal coeff
	  vals.push_back( Tripletd(i,j,-timeStepSec) ); // up neighbor's coeff
	  vals.push_back( Tripletd(j,j,timeStepSec) );  // up neighbor's diag
	}
	else if (cell.neighbors[Cell::POS_Y]->cellType == Cell::AIR) {
	  vals.push_back( Tripletd(i,i,timeStepSec) );
	}
	break;
	
      default:
	break;
      }
    }
  A.setFromTriplets(vals.begin(), vals.end());

  // Solve for the new pressure values, p.
  VectorXd p(dim);
  ConjugateGradient< SparseMatrix<double,RowMajor> > cg;
  cg.compute(A);
  p = cg.solve(b);
  if (cg.info() == Success)
    std::cout << "SUCCESS: Convergence!" << std::endl;
  else 
    std::cout << "FAILED: No Convergence..." << std::endl;
  //  std::cout << "#iterations:     " << cg.iterations() << std::endl;
  //  std::cout << "estimated error: " << cg.error()      << std::endl;  
  //  std::cout << "A: " << std::endl << A << std::endl;
  //  std::cout << "Pressure: " << std::endl << p << std::endl;
  //  std::cout << "Divergence: " << std::endl << b << std::endl;
  //  std::cout << "Ap: " << std::endl << A*p << std::endl;

  // Set new pressure values.
  for (unsigned y = 0; y < height; ++y)
    for (unsigned x = 0; x < width; ++x) {
      unsigned index = y * width + x;
      _grid(x, y).pressure = p(index);
    }

  // Modify velocity field based on updated pressure scalar field.
  for (unsigned y = 0; y < height; ++y)
    for (unsigned x = 0; x < width; ++x) {
      Cell &cell = _grid(x,y);
      float pressureVel = timeStepSec * cell.pressure;
      if (cell.cellType == Cell::FLUID) {
	// Update all neighboring velocities touched by this pressure.
	cell.vel[Cell::X] -= pressureVel;
	cell.vel[Cell::Y] -= pressureVel;
	if (cell.neighbors[Cell::POS_X])
	  cell.neighbors[Cell::POS_X]->vel[Cell::X] += pressureVel;
	if (cell.neighbors[Cell::POS_Y])
	  cell.neighbors[Cell::POS_Y]->vel[Cell::Y] += pressureVel;
      }
    }

  // Calculate the negative divergence throughout the simulation.
  for (unsigned y = 0; y < height; ++y)
    for (unsigned x = 0; x < width; ++x) {
      unsigned index = y * width + x;
      b(index) = -_grid.getVelocityDivergence(x, y);
    }
  std::cout << "New Divergence: " << std::endl << b << std::endl;
}


void FluidSolver::boundaryCollide()
{
  // Set all boundary velocities to zero in the MAC grid.
  // This prevents fluid from entering or exiting through the walls of the
  // simulation.
  const unsigned rows = _grid.getRowCount();
  const unsigned cols = _grid.getColCount();

  // Bottom row. Set velocity Y component to 0.
  // Top row. Set velocity X and Y components to 0. Set to SOLID.
  for (unsigned col = 0; col < cols; ++col) {
    _grid(col, 0).vel[Cell::Y] = 0.0f;
    _grid(col, rows-1).vel[Cell::X] = 0.0f;
    _grid(col, rows-1).vel[Cell::Y] = 0.0f;
    _grid(col, rows-1).cellType = Cell::SOLID;
  }

  // Left column. Set velocity X component to 0.
  // Right column. Set velocity X and Y components to 0. Set to SOLID.
  for (unsigned row = 0; row < rows; ++row) {
    _grid(0, row).vel[Cell::X] = 0.0f;
    _grid(cols-1, row).vel[Cell::X] = 0.0f;
    _grid(cols-1, row).vel[Cell::Y] = 0.0f;
    _grid(cols-1, row).cellType = Cell::SOLID;
  }
}


void FluidSolver::moveParticles(float timeStepSec)
{
  // Advect velocity using simple forward Euler.
  vector<Vector2>::iterator itr = _particles.begin();
  for (; itr != _particles.end(); ++itr) {
    *itr += _grid.getVelocity(*itr) * timeStepSec;
  }
}


void FluidSolver::markCells()
{
  // Sweep over all FLUID cells, resetting them to AIR.
  unsigned cellCount = _grid.getRowCount() * _grid.getColCount();
  for (unsigned i = 0; i < cellCount; ++i)
    if (_grid[i].cellType == Cell::FLUID)
      _grid[i].cellType = Cell::AIR;
  
  // Iterate over all marker particles, setting their resident cells to FLUID.
  vector<Vector2>::iterator itr = _particles.begin();
  for (; itr != _particles.end(); ++itr) {
    if (itr->x >= 0.0f && itr->x < _width &&
	itr->y >= 0.0f && itr->y < _height)
      _grid(itr->x, itr->y).cellType = Cell::FLUID;
  }
}


void FluidSolver::draw(IFluidRenderer *renderer)
{
  // If a new frame is ready for rendering, draw it.
  if (_frameReady) {
    renderer->drawGrid(_grid, _particles);
    _frameReady = false;
  }
  else {
    // TODO, redraw previous frame.
  }
}


float FluidSolver::getSimulationWidth() const
{
  return _width;
}
  

float FluidSolver::getSimulationHeight() const
{
  return _height;
}
