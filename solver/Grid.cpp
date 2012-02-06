#include "Grid.h"
#include "Cell.h"
#include <vector>
#include <cstddef>

using namespace std;


Grid::Grid(unsigned rowCount, unsigned colCount)
  : _rowCount(rowCount),
    _colCount(colCount)
{
  _cells.resize(_rowCount * _colCount);
  setCellLinkage();
}


Grid::Grid(const Grid &grid)
  : _rowCount(grid._rowCount),
    _colCount(grid._colCount)
{
  _cells = grid._cells;
  setCellLinkage();
}


Grid & Grid::operator=(const Grid &grid)
{
  if (this != &grid) {
    _rowCount = grid._rowCount;
    _colCount = grid._colCount;
    _cells = grid._cells;
    setCellLinkage();
  }
  return *this;
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

Vector<2,float> Grid::getVelocity(float x, float y) const
{
  Vector<2,float> result;
  result(0) = bilerpVel(x, y, Cell::X);
  result(1) = bilerpVel(x, y, Cell::Y); 
  return result;
}

void Grid::setCellLinkage() 
{
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
      // All neighbors are defined
      _cells[i].allNeighbors = true;
    }
  }
}

float Grid::bilerpVel(float x, float y, Cell::Dimension dim) const
{
  int i, j = 0;
  float fx, fy = 0;

  switch(dim) {
    case Cell::X:
      y -= 0.5;
      break;

    case Cell::Y:
      x -= 0.5;
      break;

    default:
      // TODO: Put error message here in the future
      break;
  }

  i = floor(x);
  j = floor(y);
  fx = x - i;
  fy = y - j;
  
  printf("%f,%f\n", x, y);

  float thisVel = (*this)(i,j).vel[dim];
  float rightVel = (*this)(i,j).neighbors[Cell::POS_X]->vel[dim];
  float topVel = (*this)(i,j).neighbors[Cell::POS_Y]->vel[dim];
  float topRightVel = (*this)(i,j).neighbors[Cell::POS_XY]->vel[dim];

  return thisVel*(1-fx)*(1-fy)+
         rightVel*fx*(1-fy)+
         topVel*(1-fx)*fy+
         topRightVel*fx*fy;
}



