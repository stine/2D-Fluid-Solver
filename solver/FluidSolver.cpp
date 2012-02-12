#include <vector>
#include "FluidSolver.h"
#include "IFluidRenderer.h"
#include "Grid.h"
#include "Cell.h"

FluidSolver::FluidSolver(const Grid &grid)
  : _width(grid.getWidth()),
    _height(grid.getHeight()),
    _grid(grid),
    _maxVelocity()
{
  // Get the max velocity within the grid.
  // TODO
}
    

void FluidSolver::draw(IFluidRenderer *renderer) const
{
  // Pass a reference to our private grid into the specified renderer.
  renderer->drawGrid(_grid);
}


float FluidSolver::getSimulationWidth() const
{
  return _width;
}
  

float FluidSolver::getSimulationHeight() const
{
  return _height;
}
