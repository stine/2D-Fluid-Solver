#ifndef __GRID_H__
#define __GRID_H__

#include <vector>
#include "Cell.H"
#include "Vector.H"

template<class T = float>
class Grid2D {
  std::vector< Cell<2, T> > cells; // Holds all cells.
  Vector<2, T> maxVelocity;        // Tracks the maximum velocity in the grid.

public:
  // Constructor
  Grid2D(unsigned xSize, unsigned ySize);

  // Get the interpolated velocity at the real location (x,y).
  Vector<2, T> getVelocity(float x, float y) const;
  
  // Get the pressure gradient at the cell (x,y).
  Vector<2, T> getPressureGradient(unsigned x, unsigned y) const;
  
protected:
  // Return a reference to the specified Cell.
  inline Cell<2, T>& operator()(unsigned x, unsigned y);

  // Return the 'index' component of velocity at the real location (x,y).
  Vector<2, T> getInterpolatedValue(float x, float y, unsigned index) const;

private:
  // Hidden default constructor
  Grid2D();
};


template<class T>
Grid2D<T>::Grid2D(unsigned xSize, unsigned ySize)
{
  cells.resize(xSize * ySize);
}


// TODO define more methods here.


#endif //__GRID_H__
