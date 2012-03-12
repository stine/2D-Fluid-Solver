// DEBUG
#include <iostream>
#include <vector>
#include "FluidSolver.h"
#include "IFluidRenderer.h"
#include "Grid.h"
#include "Cell.h"
#include "Vector2.h"
#include "SignalRelay.h"


using std::vector;

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
  for (float y = 0; y < _height; ++y) 
    for (float x = 0; x < _width; ++x) {
      grid(x,y).cellType = Cell::FLUID;
      grid(x,y).pressure = 1.0f;
      grid(x,y).vel[Cell::X] = sin(x * 45.215 + y * 88.15468) / 2; // arbitrary constants
      grid(x,y).vel[Cell::Y] = sin(x * 2.548 + y * 121.1215) / 2;  // arbitrary constants

      // Initialize particle positions
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
  Vector2 gravity(0.0f, -0.098);  // Gravity: -0.098 cells/sec^2

  advectVelocity(timeStepSec);
  applyGlobalVelocity(gravity * timeStepSec);
  boundaryCollide();
  pressureSolve();
  moveParticles(timeStepSec);
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
  Vector2 tempPos;
  float dist;
  bool intersectX = position.x < 0 || position.x > _grid.getWidth();
  bool intersectY = position.y < 0 || position.y > _grid.getHeight();

  if (intersectX && intersectY) {
    dist = position.x / -velocity.x;
    tempPos.y = position.y + dist * velocity.y;
    if (position.x < 0)
      tempPos.zeroX();
    else
      tempPos.x = _grid.getWidth();
    dist = position.y / -velocity.y;
    position.x = position.x + dist * velocity.x;
    if (position.y < 0)
      position.zeroY();
    else
      position.y = _grid.getHeight();
    if (tempPos.magnitude() < position.magnitude())
      return tempPos;
    else
      return position; 
  }
  else if (intersectX) {
    dist = position.x / -velocity.x;
    position.y = position.y + dist * velocity.y;
    if (position.x < 0)
      position.zeroX();
    else
      position.x = _grid.getWidth();
    return position;
  }  
  else if (intersectY) {
    dist = position.y / -velocity.y;
    position.x = position.x + dist * velocity.x;
    if (position.y < 0)
      position.zeroY();
    else
      position.y = _grid.getHeight();
    return position;
  }
  else 
    return position + velocity;
}

void FluidSolver::applyGlobalVelocity(Vector2 velocity)
{
  // Apply the provided velocity to all cells in the simulation.
  unsigned cellCount = _grid.getRowCount() * _grid.getColCount();
  for (unsigned i = 0; i < cellCount; ++i) {
    _grid[i].vel[Cell::X] += velocity.x;
    _grid[i].vel[Cell::Y] += velocity.y;
  }
}


void FluidSolver::pressureSolve()
{
  // TODO
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
  for (vector<Vector2>::iterator itr = _particles.begin();
                                 itr != _particles.end();
                                 ++itr)
  {
    *itr += _grid.getVelocity(*itr) * timeStepSec;
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
