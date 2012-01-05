#include "Grid.h"
#include "Cell.h"
#include <vector>
#include <cstddef>

using namespace std;


Grid::Grid(unsigned rowCount, unsigned colCount)
  : _rowCount(rowCount),
    _colCount(colCount)
{
  // Initialize data members.
  _cells.resize(_rowCount * _colCount);

  // Create linkage from each cell to its +X, +Y, and +XY neighbors.
  // Since the Cell struct's default constructor initializes NULL for
  // all neighbors, we only need to touch the cells that have these
  // neighbors.  This includes all cells EXCEPT for the last column
  // (x == _colCount - 1) and the last row (y == _rowCount - 1).
  // We account for this in our loop condition.
  for (unsigned y = 0; y < _rowCount - 1; ++y) {
    for (unsigned x = 0; x < _colCount - 1; ++x) {
      unsigned i = y * _colCount + x;
      // Right Neighbor
      _cells[i].neighbors[Cell::POS_X] = &(_cells[i + 1]);
      // Top Neighbor
      _cells[i].neighbors[Cell::POS_Y] = &(_cells[i + _colCount]);
      // Top Right Cell
      _cells[i].neighbors[Cell::POS_XY] = &(_cells[i + _colCount + 1]);
    }
  }
}


Grid::~Grid()
{
  // Tear down linkage between cells.
  vector<Cell>::iterator itr = _cells.begin();
  for( ; itr != _cells.end(); ++itr) {
    for (unsigned i = 0; i < Cell::NEIGHBOR_COUNT; ++i) {
      itr->neighbors[i] = NULL;
    }
  }
}
