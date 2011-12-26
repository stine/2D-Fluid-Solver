#include "FluidSolver.h"
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
    
