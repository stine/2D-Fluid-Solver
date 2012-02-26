// DEBUG
#include <iostream>
#include <vector>
#include "FluidSolver.h"
#include "IFluidRenderer.h"
#include "Grid.h"
#include "Cell.h"

FluidSolver::FluidSolver(const Grid &grid)
  : _width(grid.getWidth()),
    _height(grid.getHeight()),
    _grid(grid),
    _frameReady(false)
{}


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
//  boundaryCollide();
  pressureSolve();
  moveParticles(timeStepSec);
}


void FluidSolver::advectVelocity(float timeStepSec)
{
  for(float x = 0; x < _grid.getWidth(); x += 1.0f)
    for( float y = 0.5; y < _grid.getHeight(); y += 1.0f) {
      Cell &cell = _grid(floor(x), floor(y));
      Vector2 position(x, y); 
      position += _grid.getVelocity(x, y) * -timeStepSec;
      if (position.x < 0)
        position.x = 0;
      if (position.y > _grid.getWidth())
        position.x = _grid.getWidth(); 
      if (position.y < 0)
        position.y = 0;
      if (position.y > _grid.getHeight())
        position.y = _grid.getHeight();
      cell.stagedVel[Cell::X] = _grid.getVelocity(position.x, position.y).x;
    }
  for(float y = 0; y < _grid.getHeight(); y += 1.0f)
    for(float x = 0.5; x < _grid.getWidth(); x+= 1.0f) {
      Cell &cell = _grid(floor(x), floor(y));
      Vector2 position(x, y); 
      position += _grid.getVelocity(x, y) * -timeStepSec;
      if (position.x < 0)
        position.x = 0;
      if (position.y > _grid.getWidth())
        position.x = _grid.getWidth(); 
      if (position.y < 0)
        position.y = 0;
      if (position.y > _grid.getHeight())
        position.y = _grid.getHeight();
      cell.stagedVel[Cell::Y] = _grid.getVelocity(position.x, position.y).y;
    }
  for(unsigned i = 0; i < _grid.getRowCount() * _grid.getColCount(); i++) {
    _grid[i].commitStagedVel();
  }
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
  // TODO
}


void FluidSolver::draw(IFluidRenderer *renderer)
{
  // If a new frame is ready for rendering, draw it.
  if (_frameReady) {
    renderer->drawGrid(_grid);
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
