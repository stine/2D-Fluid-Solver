#include "Grid.h"
#include "Cell.h"
#include "Vector2.h"
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


Vector2 Grid::getVelocity(Vector2 position) const
{
  // Since the X and Y components of velocity are stored at different locations
  // in world space, a bilinear interpolation must be performed per-component
  // to determine the velocity at any point in the MAC grid.
  Vector2 result;
  result.x = bilerpVel(position, Cell::X);
  result.y = bilerpVel(position, Cell::Y);
  return result;
}


float Grid::getVelocityDivergence(unsigned x, unsigned y) const
{
  // Get reference to the cell. Note that x and y are assumted to be valid.
  Cell cell = (*this)(x,y);

  // If all neighbors are present, get the velocity value from each.
  float xDivergence;
  float yDivergence;
  if (cell.allNeighbors) {
    xDivergence = cell.neighbors[Cell::POS_X]->vel[Cell::X] - cell.vel[Cell::X];
    yDivergence = cell.neighbors[Cell::POS_Y]->vel[Cell::Y] - cell.vel[Cell::Y];
  }
  // Else, set missing neighbors to 0 velocity.
  else {
    // Get right/top neighbor's velocity, or set to 0.
    float rightXVel = cell.neighbors[Cell::POS_X]
      ? cell.neighbors[Cell::POS_X]->vel[Cell::X] : 0.0f;
    float topYVel = cell.neighbors[Cell::POS_Y]
      ? cell.neighbors[Cell::POS_Y]->vel[Cell::Y] : 0.0f;

    // Calculate x and y divergence.
    xDivergence = rightXVel - cell.vel[Cell::X];
    yDivergence = topYVel - cell.vel[Cell::Y];
  }
  
  return xDivergence + yDivergence;
}


Vector2 Grid::getMaxVelocity() const
{
  // Iterate through all MAC cell centers, finding the maximum velocity.
  // Note that this is an incredibly naive and expensive approach to 
  // estimating the maximum velocity in the grid.  Consider revising
  // this in the future.
  Vector2 maxVel;
  for (float y = 0.5f; y < getHeight(); ++y)
    for (float x = 0.5f; x < getWidth(); ++x) {
      Vector2 vel = getVelocity(Vector2(x, y));
      if (vel.magnitude() > maxVel.magnitude())
	maxVel = vel;
    }
  
  return maxVel;
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


float Grid::bilerpVel(Vector2 position, Cell::Dimension dim) const
{
  // Ensure that incoming x and y values are not greater than the
  // max grid width. Do this prior to the "shifting" step below,
  // since a clamped value will need to be adjusted for X or Y
  // MAC grid sampling.
  if (position.x > getWidth())
    position.x = getWidth();
  if (position.y > getHeight())
    position.y = getHeight();

  // In a MAC Grid, the X and Y velocities are offset from the centerpoint
  // of a MAC cell by half the cell width.  In other words, if the center
  // of a cell is at local (0.5, 0.5), the X velocity for the cell is
  // sampled and stored at (0, 0.5), and the Y velocity for the cell is
  // sampled and stored at (0.5, 0).  This is to help with calculating
  // an accurate central difference across the cell.
  // Here, an offset is applied to the provided X or Y coordinate to
  // adjust for this velocity component offset in bilinear interpolation.
  // See documentation on the MAC grid for a deeper explanation.
  switch(dim) {
    case Cell::X:
      position.y -= 0.5;
      break;
    case Cell::Y:
      position.x -= 0.5;
      break;
    default:
      break;
  }

  // Ensure the resulting X and Y value are not less than the minimal
  // grid index.  Do this after the "shifting" step above, since the
  // shifted result may be less than 0.0.
  if (position.x < 0.0f)
    position.zeroX();
  if (position.y < 0.0f)
    position.zeroY();

  // Determine the base and fractional cell index for interpolation.
  unsigned i, j;

  i = floor(position.x);
  j = floor(position.y);
  position -= Vector2(i,j);

  // Get the base cell.
  Cell cell = (*this)(i,j);

  float thisVel, rightVel, topVel, topRightVel;
  // If all neighbors are present, get the velocity value from each.
  if (cell.allNeighbors) {
    thisVel  = cell.vel[dim];
    rightVel = cell.neighbors[Cell::POS_X]->vel[dim];
    topVel   = cell.neighbors[Cell::POS_Y]->vel[dim];
    topRightVel = cell.neighbors[Cell::POS_XY]->vel[dim];
  }
  // Else, set missing neighbors to 0 velocity.
  else {
    thisVel  = cell.vel[dim];
    rightVel = cell.neighbors[Cell::POS_X]
      ? cell.neighbors[Cell::POS_X]->vel[dim] : 0;
    topVel   = cell.neighbors[Cell::POS_Y]
      ? cell.neighbors[Cell::POS_Y]->vel[dim] : 0;
    topRightVel = cell.neighbors[Cell::POS_XY]
      ? cell.neighbors[Cell::POS_XY]->vel[dim] : 0;
  }

  // Perform the bilinear interpolation.
  return bilerp(position, thisVel, rightVel, topVel, topRightVel);
}
