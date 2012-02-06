#ifndef __GRID_H__
#define __GRID_H__

#include <vector>
#include "Cell.h"
#include "Vector.h"


class Grid {
  unsigned _rowCount;  // The number of rows in the sim.
  unsigned _colCount;  // The number of columns in the sim.
  std::vector<Cell> _cells;  // STL Vector of all managed cells.

public:
  // Constructs an instance of Grid of size rowCount by colCount. Currently
  // each cell is hardcoded at 1.0f units by 1.0f units in size.
  //
  // Arguments:
  //   unsigned rowCount - The number of rows of cells in the simulation. 
  //   unsigned colCount - The number of columns of cells in the simulation.
  Grid(unsigned rowCount, unsigned colCount);

  // Copy constructs an instance of Grid.
  //
  // Arguments:
  //   Grid &grid - The grid to duplicate.
  Grid(const Grid &grid);

  // Assignment operator for a grid object.
  //
  // Arguments:
  //   Grid &grid - The grid to assign.
  // Returns:
  //   Grid & - A reference to this Grid.
  Grid & operator=(const Grid &grid);

  // Destructs an instance of Grid.
  //
  // Arguments:
  //   None
  ~Grid();

  // Returns a reference to the specified cell, as specified by x and y coord.
  // 
  // Arguments:
  //   unsigned x - The integer x coordinate of this cell within the grid.
  //   unsigned y - The integer y coordinate of this cell within the grid.
  //
  // Returns:
  //   Cell& - A reference to the specified cell.
  inline Cell& operator()(unsigned x, unsigned y);
  
  // Returns a copy of the specified cell, as specified by x and y coord.
  // 
  // Arguments:
  //   unsigned x - The integer x coordinate of this cell within the grid.
  //   unsigned y - The integer y coordinate of this cell within the grid.
  //
  // Returns:
  //   Cell - A copy of the specified cell.
  inline Cell operator()(unsigned x, unsigned y) const;

  // Returns a reference to the specified cell, as specified by an index
  // into a contiguous array of Cell objects in row-major order.
  // 
  // Arguments:
  //   unsigned index - The index into the row-major
  //
  // Returns:
  //   Cell& - A reference to the specified cell.
  inline Cell& operator[](unsigned index);

  // Returns a copy of the specified cell, as specified by an index
  // into a contiguous array of Cell objects in row-major order.
  // 
  // Arguments:
  //   unsigned index - The index into the row-major
  //
  // Returns:
  //   Cell - A copy of the specified cell.
  inline Cell operator[](unsigned index) const;

  // Get the velocity at a location within the grid. This performs a bilinear
  // interpolation between values in neighboring cells per component.
  // 
  // Arguments:
  //   float x - The x coordinate within this cell to sample from.
  //   float y - The y coordinate within this cell to sample from.
  //
  // Returns:
  //   Vector<2,float> - The interpolated velocity at this point.
  Vector<2,float> getVelocity(float x, float y) const;

  // Calculates the pressure gradient across this cell. 
  // 
  // Arguments:
  //   unsigned x - The integer x coordinate of this cell within the grid.
  //   unsigned y - The integer y coordinate of this cell within the grid.
  //
  // Returns:
  //   Vector<2,float> - The pressure gradient across this cell.
  Vector<2,float> getPressureGradient(unsigned x, unsigned y) const;

  // Calculates the divergence of the velocity field within this cell.
  // 
  // Arguments:
  //   unsigned x - The integer x coordinate of this cell within the grid.
  //   unsigned y - The integer y coordinate of this cell within the grid.
  //
  // Returns:
  //   float - The divergence of the velocity field within this cell.
  float getVelocityDivergence(unsigned x, unsigned y) const;

  // Gets the number of rows in the grid.
  //
  // Arguments:
  //   None
  // Returns:
  //   unsigned - The number of rows in the grid.
  inline unsigned getRowCount() const;

  // Gets the number of cols in the grid.
  //
  // Arguments:
  //   None
  // Returns:
  //   unsigned - The number of cols in the grid.
  inline unsigned getColCount() const;

private:
  // Creates linkage between neighboring cells.
  void setCellLinkage();

  // Calculates bilinear interpolation for an x and y position
  float bilerpVel(float x, float y, Cell::Dimension dim) const;
};


Cell& Grid::operator()(unsigned x, unsigned y)
{
  unsigned index = y * _colCount + x;
  return _cells[index];
}
 
 
Cell Grid::operator()(unsigned x, unsigned y) const
{
  unsigned index = y * _colCount + x;
  return _cells[index];
}


Cell& Grid::operator[](unsigned index)
{
  return _cells[index];
}


Cell Grid::operator[](unsigned index) const
{
  return _cells[index];
}


unsigned Grid::getRowCount() const
{
  return _rowCount;
}


unsigned Grid::getColCount() const
{
  return _colCount;
}

#endif //__GRID_H__
