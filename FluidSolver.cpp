#include "FluidSolver.h"
#include "FluidRenderer2D.h"
#include <vector>

FluidSolver2D::FluidSolver2D(unsigned iSize, unsigned jSize,
			     const std::vector<bool> &initialFluid)
  : _iSize(iSize),
    _jSize(jSize),
    _grid(iSize, jSize)
{
  // TODO initialize _maxVelocity.

  // TODO setup initial fluid state.
}
    

void FluidSolver2D::draw(FluidRenderer2D *renderer) const
{
  // Pass a reference to our private grid into the specified renderer.
  renderer->drawGrid2D(_grid);
}

