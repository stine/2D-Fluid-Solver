#ifndef __GRID_H__
#define __GRID_H__

#include <vector>
#include "Cell.h"
#include "Vector2.h"


class Grid {
  unsigned _rowCount;  // The number of rows in the sim.
  unsigned _colCount;  // The number of columns in the sim.
  std::vector<Cell> _cells;  // STL Vector of all managed cells.
  const static unsigned _minSize = 3; // Minimum size of grid in any dim.

public:
  // Constructs an instance of Grid of size width by height. Width and height
  // are to be provided in world coordinates.  The Grid class produces a MAC
  // grid of size that rounds up to the nearest integer in each dimension.
  //
  // Arguments:
  //   float width - The width of the simulation in world coordinates.
  //   float height - The height of the simulation in world coordinates.
  Grid(float width, float height);

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
  // NOTE: x and y are expected to be range supplied at construction time
  // of this Grid class, where x = [0.0f, width), y = [0.0f, height)
  // 
  // Arguments:
  //   Vector2 position - The position to sample velocity at
  // Returns:
  //   Vector2 - The interpolated velocity at this point.
  Vector2 getVelocity(Vector2 position) const;

  // Calculates the pressure gradient across this cell. 
  // 
  // Arguments:
  //   unsigned x - The integer x coordinate of this cell within the grid.
  //   unsigned y - The integer y coordinate of this cell within the grid.
  //
  // Returns:
  //   Vector2 - The pressure gradient across this cell.
  Vector2 getPressureGradient(unsigned x, unsigned y) const;

  // Calculates the divergence of the velocity field within this cell.
  // NOTE: x and y are expected to be range supplied at construction time
  // of this Grid class, where x = [0.0f, width), y = [0.0f, height)
  // 
  // Arguments:
  //   unsigned x - The integer x coordinate of this cell within the grid.
  //   unsigned y - The integer y coordinate of this cell within the grid.
  //
  // Returns:
  //   float - The divergence of the velocity field within this cell.
  float getVelocityDivergence(unsigned x, unsigned y) const;

  // Calculates the maximum velocity in the grid by sampling the center
  // of each MAC cell.  This is currently an expensive operation due to
  // naive implementation.
  //
  // Arguments:
  //   None
  //
  // Returns:
  //   Vector2 - The maximum velocity since resetMaxVelocity().
  Vector2 getMaxVelocity() const;

  // Gets the simulation height supported by this grid, in world coordinates.
  //
  // Arguments:
  //   None
  //
  // Returns:
  //   float - The simulation height supported by this grid.
  inline float getHeight() const;

  // Gets the simulation width supported by this grid, in world coordinates.
  //
  // Arguments:
  //   None
  //
  // Returns:
  //   float - The simulation width supported by this grid.
  inline float getWidth() const;

  // Gets the number of rows in the MAC grid.
  // NOTE: The top row of the grid exceeds the bounds of the simulation.
  // This row is used to store the y-velocities at the top of the sim.
  // To determine the height of the simulation that is represented by this
  // grid, use getHeight() instead.
  //
  // Arguments:
  //   None
  //
  // Returns:
  //   unsigned - The number of rows in the grid.
  inline unsigned getRowCount() const;

  // Gets the number of columns in the MAC grid.
  // NOTE: The far right of the grid exceeds the bounds of the simulation.
  // This column is used to store the y-velocities at the top of the sim.
  // To determine the height of the simulation that is represented by this
  // grid, use getHeight() instead.
  //
  // Arguments:
  //   None
  //
  // Returns:
  //   unsigned - The number of cols in the grid.
  inline unsigned getColCount() const;

private:
  // Creates linkage between neighboring cells.
  void setCellLinkage();

  // Calculates a velocity component at the given world location in the MAC grid.
  float bilerpVel(Vector2 position, Cell::Dimension dim) const;

  // Utility function to perform bilinear interpolation between four values.
  // 
  // Arguments:
  //   Vector2 position - Position to perform bilinear interpolation at
  //   float originVal - Value at (0, 0)
  //   float posXVal   - Value at (0, 1)
  //   float posYVal   - Value at (1, 0)
  //   float posXYVal  - Value at (1, 1)
  // Returns:
  //   float - Bilinearly interpolated value at the specified location.
  inline float bilerp(Vector2 position,
		      float originVal, float posXVal,
		      float posYVal, float posXYVal) const;
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


float Grid::getHeight() const
{
  return static_cast<float>(_rowCount - 1);
}


float Grid::getWidth() const
{
  return static_cast<float>(_colCount - 1);
}


unsigned Grid::getRowCount() const
{
  return _rowCount;
}


unsigned Grid::getColCount() const
{
  return _colCount;
}


float Grid::bilerp(Vector2 pos,
		   float originVal, float posXVal,
		   float posYVal, float posXYVal) const 
{
  return (1-pos.x) * (1-pos.y) * originVal +
         pos.x     * (1-pos.y) * posXVal +
         (1-pos.x) * pos.y     * posYVal +
         pos.x     * pos.y     * posXYVal;
}


#endif //__GRID_H__
