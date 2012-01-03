#include "FluidSolver.h"
#include "FluidRenderer.h"
#include <vector>

FluidSolver2D::FluidSolver2D(unsigned rowCount, unsigned colCount,
			     const std::vector<bool> &initialFluid)
  : _rowCount(rowCount),
    _colCount(colCount),
    _grid(rowCount, colCount),
    _maxVelocity()
{
  // Set initial fluid state within the 2D grid.
  for (unsigned i = 0; i < initialFluid.size(); ++i) {
    unsigned rowIdx = i / _colCount;
    unsigned colIdx = i % _colCount;
    _grid(rowIdx, colIdx).setIsLiquid(initialFluid[i]);
  }
}
    

void FluidSolver2D::draw(FluidRenderer *renderer) const
{
  // Pass a reference to our private grid into the specified renderer.
  renderer->drawGrid2D(_grid);
}


unsigned FluidSolver2D::getSimulationWidth() const
{
  return _colCount;
}
  

unsigned FluidSolver2D::getSimulationHeight() const
{
  return _rowCount;
}
