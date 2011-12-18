#ifndef __GRID_H__
#define __GRID_H__

#include <vector>
#include "Cell.H"

class Grid2D {
  const unsigned _iSize;       // The number of rows in the sim.
  const unsigned _jSize;       // The number of columns in the sim.
  std::vector<Cell2D> _cells;  // STL Vector of all managed cells.

public:
  // Constructs an isntance of Grid2D of size iSize by jSize. Currently
  // each cell is 1.0f units by 1.0f units in size.
  //
  // Arguments:
  //   unsigned iSize - The number of rows of cells in the simulation.
  //   unsigned jSize - The number of columns of cells in the simulation.
  Grid2D(unsigned iSize, unsigned jSize);

  // Returns a reference to the specified cell.
  // 
  // Arguments:
  //   unsigned i - The i index (row) of the cell.
  //   unsigned j - The j index (column) of the cell.
  //
  // Returns:
  //   Cell2D& - A reference to the specified cell.
  inline Cell2D& operator()(unsigned i, unsigned j);
  
  // Returns a copy of the specified cell.
  // 
  // Arguments:
  //   unsigned i - The i index (row) of the cell.
  //   unsigned j - The j index (column) of the cell.
  //
  // Returns:
  //   Cell2D - A copy of the specified cell.
  inline Cell2D operator()(unsigned i, unsigned j) const;

private:
  // Hidden default constructor
  Grid2D();
};


#endif //__GRID_H__
