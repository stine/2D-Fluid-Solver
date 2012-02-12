#ifndef __CELL_H__
#define __CELL_H__

#include <cstddef>
#include <cstring>

// Note that a Vector<> is not used to represent velocity inside a cell. This
//   is because in a MAC (Marker-and-Cell) Grid, each velocity component is
//   stored normal to a different face on the containing cell.
struct Cell {
  // Enumerated type to determine contents of Cell.
  enum Type {
    AIR = 0,
    FLUID,
    SOLID,
    TYPE_COUNT
  };
  // Enumerated type used in the Cell struct to enumerate Neighbors.
  enum Neighbor {   
    POS_X = 0,
    POS_Y,
    POS_XY,
    NEIGHBOR_COUNT
  };
  // Enumerated type to define the dimensionality of the Cell.
  enum Dimension {
    X = 0,
    Y,
    DIM_COUNT
  };


  // Public data members.
  float pressure;             // Pressure, as sampled at the center of this cell
  float vel[DIM_COUNT];       // Velocity component, as sampled at the faces.
  float stagedVel[DIM_COUNT]; // Temp vel. component, as sampled at faces.
  Type  cellType;             // Contents type of this cell.
  bool  allNeighbors;         // Flag, true if all four neighbors are in grid.
  Cell *neighbors[NEIGHBOR_COUNT]; // Pointers to neighbor cells.


  // Default Constuctor, simply initializes data members to sensible values.
  //
  // Arguments:
  //   None
  Cell();

  // Realizes the staged velocity (previously set via stageVel) as the cell's
  // current velocity. "stagedVel"'s value isn't modified.
  //
  // Arguments:
  //   None
  // 
  // Returns:
  //   None
  void commitStagedVel();
};


#endif //__CELL_H__
