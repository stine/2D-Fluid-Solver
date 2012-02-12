#include "Grid.h"
#include "Cell.h"
#include <vector>
#include <cstddef>

using namespace std;

Grid::Grid(float width, float height)
{
  // Note the extra top/right border cell to track velocity at edges of sim.
  _colCount = width  < _minSize ? _minSize : ceil(width)  + 1;
  _rowCount = height < _minSize ? _minSize : ceil(height) + 1;
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
  unsigned x, y;

  // Create linkage from each cell to its +X, +Y, and +XY neighbors.
  // Keep in mind that the Cell struct's default constructor initializes
  // NULL for all neighbor pointers, so we only need to touch the values
  // that must be changed.
  // First, set linkage for the cells that have all neighbors.
  for (y = 0; y < _rowCount - 1; ++y) {
    for (x = 0; x < _colCount - 1; ++x) {
      Cell &cell = (*this)(x,y);
      cell.neighbors[Cell::POS_X]  = &(*this)(x+1, y);
      cell.neighbors[Cell::POS_Y]  = &(*this)(x,   y+1);
      cell.neighbors[Cell::POS_XY] = &(*this)(x+1, y+1);
      cell.allNeighbors = true;
    }
  }
  // Set linkage for the cells in the far right column, but not the corner.
  x = _colCount - 1;
  for (y = 0; y < _rowCount - 1; ++y) {
    (*this)(x,y).neighbors[Cell::POS_Y] = &(*this)(x, y+1);
  }

  // Set linkage for the cells in the top row, but not the corner.
  y = _rowCount - 1;
  for (x = 0; x < _colCount - 1; ++x) {
    (*this)(x,y).neighbors[Cell::POS_X] = &(*this)(x+1, y);
  }
}

float Grid::bilerpVel(float x, float y, Cell::Dimension dim) const
{
  // Ensure that incoming x and y values are not greater than the
  // max grid width. Do this prior to the "shifting" step below,
  // since a clamped value will need to be adjusted for X or Y
  // MAC grid sampling.
  if (x > getWidth())
    x = getWidth();
  if (y > getHeight())
    y = getHeight();

  // TODO comment about offsets.
  switch(dim) {
    case Cell::X:
      y -= 0.5;
      break;
    case Cell::Y:
      x -= 0.5;
      break;
    default:
      break;
  }

  // Ensure the resulting X and Y value are not less than the minimal
  // grid index.  Do this after the "shifting" step above, since the
  // shifted result may be less than 0.0.
  if (x < 0.0f)
    x = 0.0f;
  if (y < 0.0f)
    y = 0.0f;

  // Determine the base and fractional cell index for interpolation.
  unsigned i, j = 0;
  float fi, fj = 0;
  i = floor(x);
  j = floor(y);
  fi = x - i;
  fj = y - j;

  // Get the base cell.
  Cell cell = (*this)(i,j);

  // If all neighbors are present, use direct bilinear interpolation.
  if (cell.allNeighbors) {
    float thisVel  = cell.vel[dim];
    float rightVel = cell.neighbors[Cell::POS_X]->vel[dim];
    float topVel   = cell.neighbors[Cell::POS_Y]->vel[dim];
    float topRightVel = cell.neighbors[Cell::POS_XY]->vel[dim];
    return bilerp(fi, fj, thisVel, rightVel, topVel, topRightVel);
  }
  // Else, perform additional checks 
  else {
    float thisVel  = cell.vel[dim];
    float rightVel = cell.neighbors[Cell::POS_X]
      ? cell.neighbors[Cell::POS_X]->vel[dim] : 0;
    float topVel   = cell.neighbors[Cell::POS_Y]
      ? cell.neighbors[Cell::POS_Y]->vel[dim] : 0;
    float topRightVel = cell.neighbors[Cell::POS_XY]
      ? cell.neighbors[Cell::POS_XY]->vel[dim] : 0;
    return bilerp(fi, fj, thisVel, rightVel, topVel, topRightVel);
  }
}



