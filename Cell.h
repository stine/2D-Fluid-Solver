#ifndef __CELL2D_H__
#define __CELL2D_H__

#include "Vector.h"

#define CELL2D_DIM 2
#define CELL2D_NEIGHBOR_COUNT 4

// Note that a Vector<> is not used to represent velocity inside a cell. This
//   is because in a MAC (Marker-and-Cell) Grid, each velocity component is
//   stored normal to a different face on the containing cell.

class Cell2D {
  float _pressure;          // Pressure, as sampled at the center of this cell.
  float _velocity[2];       // Velocity component, as sampled at the faces.
  float _stagedVelocity[2]; // Temp vel. component, as sampled at the faces.
  bool  _isLiquid;          // Flag, true if the cell contains liquid.
  bool  _hasAllNeighbors;   // Flag, true if all four neighbors are in grid.
  Cell2D *_neighbor[4];     // Pointers to neighbor cell, order: -X, +X, -Y, +Y.

public:
  // Default Constructor
  //
  // Sets up an empty grid cell.
  //
  // Arguments:
  //   None
  Cell2D();

  // Sets linkage from this cell to its neighboring cells.  Allows velocity
  // to be calculated at arbitrary points within this cell by interpolating
  // between neighbors.
  //
  // Note that passing a NULL pointer as an argument indicates that there is
  // no neighbor on the specified side of the cell.
  //
  // This must be done before getVelocity(), getPressureGradient(), or 
  // getVelocityDivergence() can be called.
  //
  // Arguments:
  //   Cell2D *negXNeighbor - Pointer to the neighbor on the negative x side.
  //   Cell2D *posXNeighbor - Pointer to the neighbor on the positive x side.
  //   Cell2D *negYNeighbor - Pointer to the neighbor on the negative y side.
  //   Cell2D *posYNeighbor - Pointer to the neighbor on the positive y side.
  //
  // Returns:
  //   None
  void setLinkage(Cell2D *negXNeighbor,
		  Cell2D *posXNeighbor,
		  Cell2D *negYNeighbor,
		  Cell2D *posYNeighbor);

  // Tears down linkage from this cell to its neighboring cells.
  //
  // Arguments:
  //   None
  // 
  // Returns:
  //   None
  void unsetLinkage();

  // Gets a flag determining whether or not this cell contains liquid.
  //
  // Arguments:
  //   None
  // 
  // Returns:
  //   bool - True if this cell currently contains liquid.
  bool getIsLiquid() const;

  // Get the pressure at the center of this cell.
  // 
  // Arguments:
  //   None
  //
  // Returns:
  //   float - The pressure at the center of this cell.
  float getPressure() const;

  // Gets the MAC Grid cell's [xy] velocity component.
  // 
  // Arguments:
  //   None
  //
  // Returns:
  //   float - The [xy] component of velocity at position 0.0[xy].
  float getXVelocity() const;
  float getYVelocity() const;

  // Get the velocity at a location within this cell. This performs a bilinear
  // interpolation between values in neighboring cells for each component.
  // 
  // Arguments:
  //   float x - The x coordinate within this cell to sample from. [0.0, 1.0)
  //   float y - The y coordinate within this cell to sample from. [0.0, 1.0)
  //
  // Returns:
  //   Vector<2,float> - The interpolated velocity at this point.
  Vector<2,float> getVelocity(float x, float y) const;

  // Calculates the pressure gradient across this cell. 
  // 
  // Arguments:
  //   None
  //
  // Returns:
  //   Vector<2,float> - The pressure gradient across this cell.
  Vector<2,float> getPressureGradient() const;

  // Calculates the divergence of the velocity field within this cell.
  // 
  // Arguments:
  //   None
  //
  // Returns:
  //   float - The divergence of the velocity field within this cell.
  float getVelocityDivergence() const;

  // Sets a flag determining whether or not this cell contains liquid.
  //
  // Arguments:
  //   bool isLiquid - True if this cell currently contains liquid.
  // 
  // Returns:
  //   None
  void setIsLiquid(bool isLiquid);

  // Sets the pressure at the center of this cell.
  // 
  // Arguments:
  //   float pressure - The pressure to set.
  //
  // Returns:
  //   None
  void setPressure(float pressure);

  // Sets the staggered normal velocities for this cell to the specified
  // values.  Note again that we don't use a Vector here - this is to be
  // clear that each component of the velocity is sampled at a distinct
  // location in space (Marker and Cell Grid).
  //
  // Arguments:
  //   float xVelocity - The new velocity's x component at 0.0x.
  //   float yVelocity - The new velocity's y component at 0.0y.
  //
  // Returns:
  //   None
  void setVelocity(float xVelocity, float yVelocity);

  // Stages staggered normal velocities for this cell. Note again that
  // we don't use a Vector here - this is to be clear that each component
  // of the velocity is sampled at a distinct location in space (Marker
  // and Cell Grid).
  //
  // Arguments:
  //   float xVelocity - The staged velocity's x component at 0.0x.
  //   float yVelocity - The staged velocity's y component at 0.0y.
  //
  // Returns:
  //   None
  void stageVelocity(float xVelocity, float yVelocity);
  
  // Realizes the staged velocity (previously set via stageVelocity) as
  // the cell's current velocity. After completion, the stagedVelocity is
  // zeroed out.
  //
  // Arguments:
  //   None
  // 
  // Returns:
  //   None
  void commitStagedVelocity();
};


#endif //__CELL_H__
