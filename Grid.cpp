#include "Grid.h"
#include "Cell.h"
#include <vector>
#include <cstddef>

using namespace std;

Grid2D::Grid2D(unsigned rowCount, unsigned colCount)
  : _rowCount(rowCount),
    _colCount(colCount)
{
  // Initialize data members.
  _cells.resize(_rowCount * _colCount);
  
  // Create linkage between cells, since we're delegating velocity value
  // interpolation to the cell objects themselves.
  unsigned size = _rowCount * _colCount;
  for (unsigned i = 0; i < size; ++i) {
    Cell2D *negX = NULL;
    if (i % _colCount != 0) {
      negX = &(_cells[i - 1]);
    }
    
    Cell2D *posX = NULL;
    if (i % _colCount != _colCount - 1) {
      posX = &(_cells[i + 1]);
    }

    Cell2D *negY = NULL;
    if (i / _colCount != 0) {
      negY = &(_cells[i - _colCount]);
    }

    Cell2D *posY = NULL;
    if (i / _colCount != _rowCount - 1) {
      posY = &(_cells[i + _colCount]);
    }
    
    _cells[i].setLinkage(negX, posX, negY, posY);
  }
}


Grid2D::~Grid2D()
{
  // Tear down linkage between cells.
  vector<Cell2D>::iterator itr = _cells.begin();
  for( ; itr != _cells.end(); ++itr) {
    itr->unsetLinkage();
  }

  _cells.clear();
}
