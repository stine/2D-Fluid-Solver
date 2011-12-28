#include "Grid.h"
#include <vector>

using namespace std;

Grid2D::Grid2D(unsigned iSize, unsigned jSize)
  : _iSize(iSize),
    _jSize(jSize)
{
  // Initialize data members.
  _cells.resize(_iSize * _jSize);
  // TODO maxVelocity
  
  // Create linkage between cells, since we're delegating interpolation to the
  // cell objects themselves.
}
