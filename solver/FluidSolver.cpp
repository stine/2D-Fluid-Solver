#include <vector>
#include "FluidSolver.h"
#include "IFluidRenderer.h"
#include "Grid.h"
#include "Cell.h"

FluidSolver::FluidSolver(unsigned rowCount, unsigned colCount,
			     const std::vector<bool> &initialFluid)
  : _rowCount(rowCount),
    _colCount(colCount),
    _grid(rowCount, colCount),
    _maxVelocity()
{
  // TODO Assert that the initialFluid vector size is correct.
  // Assert (rowCount * colCount == initialFluid.size()

  // Set initial fluid state within the 2D grid.
  for (unsigned i = 0; i < initialFluid.size(); ++i) {
    _grid[i].isLiquid = initialFluid[i];
  }
}


FluidSolver::FluidSolver(const Grid &grid)
  : _rowCount(grid.getRowCount()),
    _colCount(grid.getColCount()),
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


unsigned FluidSolver::getSimulationWidth() const
{
  return _colCount;
}
  

unsigned FluidSolver::getSimulationHeight() const
{
  return _rowCount;
}
