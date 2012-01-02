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
    Cell2D *neighbors[Cell2D::NEIGHBOR_COUNT];

    // Top Left Neighborx
    // TODO
    neighbors[Cell2D::TOP_LEFT] = NULL;

    // Top Neighbor
    if (i / _colCount != _rowCount - 1) {
      neighbors[Cell2D::TOP_CENTER] = &(_cells[i + _colCount]);
    }

    // Top Right Cell
    // TODO
    neighbors[Cell2D::TOP_RIGHT] = NULL;
    
    // Left Neighbor
    if (i % _colCount != 0) {
      neighbors[Cell2D::LEFT] = &(_cells[i - 1]);
    }

    // Right Neighbor
    if (i % _colCount != _colCount - 1) {
      neighbors[Cell2D::RIGHT] = &(_cells[i + 1]);
    }
    
    // Bottom Left Neighbor
    // TODO
    neighbors[Cell2D::BOTTOM_LEFT] = NULL;

    // Bottom Neighbor
    if (i / _colCount != 0) {
      neighbors[Cell2D::BOTTOM_CENTER] = &(_cells[i - _colCount]);
    }

    // Bottom Right Neighbor
    // TODO
    neighbors[Cell2D::BOTTOM_RIGHT] = NULL;

    // Set the cell linkage.
    _cells[i].setLinkage(neighbors, Cell2D::NEIGHBOR_COUNT);
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
