#ifndef __CELL_H__
#define __CELL_H__

// Note that a Vector<> is not used to represent velocity inside a cell. This
//   is because in a MAC (Marker-and-Cell) Grid, each velocity component is
//   stored normal to a different face on the containing cell.

template <unsigned N, class T = float>
struct Cell {
  T p;       // Pressure, as sampled at the center of this cell.
  T u[N];    // Velocity component, as sampled at the faces (using MAC Grid).
  T tmpU[N]; // Temp vel. component, as sampled at the faces (using MAC Grid).
public:
  // Copies "tmpU" value into "u".
  void realizeTmp();
};


#endif //__CELL_H__
