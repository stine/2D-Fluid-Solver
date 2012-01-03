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
  Cell2D *neighbors[Cell2D::NEIGHBOR_COUNT];
  unsigned size = _rowCount * _colCount;
  for (unsigned i = 0; i < size; ++i) {

    unsigned row = i / _colCount;
    unsigned col = i % _colCount;

    // Initialize all neighbors to a NULL pointer.
    for (unsigned n = 0; n < Cell2D::NEIGHBOR_COUNT; ++n) {
      neighbors[n] = NULL;
    }

    // Top Left Neighbor
    if (row != _rowCount - 1 &&
	col != 0) {
      neighbors[Cell2D::TOP_LEFT] = &(_cells[i + _colCount - 1]);
    }

    // Top Neighbor
    if (row != _rowCount - 1) {
      neighbors[Cell2D::TOP_CENTER] = &(_cells[i + _colCount]);
    }

    // Top Right Cell
    if (row != _rowCount - 1 &&
	col != _colCount - 1) {
      neighbors[Cell2D::TOP_RIGHT] = &(_cells[i + _colCount + 1]);
    }
    
    // Left Neighbor
    if (col != 0) {
      neighbors[Cell2D::LEFT] = &(_cells[i - 1]);
    }

    // Right Neighbor
    if (col != _colCount - 1) {
      neighbors[Cell2D::RIGHT] = &(_cells[i + 1]);
    }
    
    // Bottom Left Neighbor
    if (row != 0 &&
	col != 0) {
      neighbors[Cell2D::BOTTOM_LEFT] = &(_cells[i - _colCount - 1]);
    }

    // Bottom Neighbor
    if (row != 0) {
      neighbors[Cell2D::BOTTOM_CENTER] = &(_cells[i - _colCount]);
    }

    // Bottom Right Neighbor
    if (row != 0 &&
	col != _colCount - 1) {
      neighbors[Cell2D::BOTTOM_RIGHT] = &(_cells[i - _colCount + 1]);
    }

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
