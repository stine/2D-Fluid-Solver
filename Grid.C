#include "Grid.H"
#include <vector>

using namespace std;

Grid2D::Grid2D(unsigned xSize, unsigned ySize)
{
  // Initialize data members.
  _cells.resize(xSize * ySize);
  // TODO maxVelocity
  
  // Create linkage between cells, since we're delegating interpolation to the
  // cell objects themselves.
  // TODO
}


// TODO define more methods here.


