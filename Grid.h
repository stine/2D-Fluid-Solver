#ifndef __GRID_H__
#define __GRID_H__

#include <vector>
#include "Cell.h"

class Grid2D {
  const unsigned _rowCount;    // The number of rows in the sim.
  const unsigned _colCount;    // The number of columns in the sim.
  std::vector<Cell2D> _cells;  // STL Vector of all managed cells.

public:
  // Constructs an instance of Grid2D of size rowCount by colCount. Currently
  // each cell is 1.0f units by 1.0f units in size.
  //
  // Arguments:
  //   unsigned rowCount - The number of rows of cells in the simulation. 
  //   unsigned colCount - The number of columns of cells in the simulation.
  Grid2D(unsigned rowCount, unsigned colCount);

  // Destructs an instance of Grid2D.
  //
  // Arguments:
  //   None
  ~Grid2D();

  // Returns a reference to the specified cell.
  // 
  // Arguments:
  //   unsigned row - The row index of the cell.
  //   unsigned col - The column index of the cell.
  //
  // Returns:
  //   Cell2D& - A reference to the specified cell.
  inline Cell2D& operator()(unsigned row, unsigned col);
  
  // Returns a copy of the specified cell.
  // 
  // Arguments:
  //   unsigned row - The row index of the cell.
  //   unsigned col - The column index of the cell.
  //
  // Returns:
  //   Cell2D - A copy of the specified cell.
  inline Cell2D operator()(unsigned row, unsigned col) const;

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
};


Cell2D& Grid2D::operator()(unsigned row, unsigned col)
{
  unsigned index = row * _colCount + col;
  return _cells[index];
}
 
 
Cell2D Grid2D::operator()(unsigned row, unsigned col) const
{
  unsigned index = row * _colCount + col;
  return _cells[index];
}


unsigned Grid2D::getRowCount() const
{
  return _rowCount;
}


unsigned Grid2D::getColCount() const
{
  return _colCount;
}

#endif //__GRID_H__
