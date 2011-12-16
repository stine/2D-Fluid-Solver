#include <iostream>
#include "Grid.H"

int main(int argc, char **argv)
{
  // 0. Set initial simulation state.
  // 1. Calculate the simulation time step based on max velocity (CFL Condition)
  // 2. Advance the velocity field.
  //    2a. Apply convection using backward particle trace and RK2
  //    2b. Apply external forces, inc gravity.
  //    2c. Run the pressure solve through MUMPS.
  //    2d. Apply the pressure to the simulation.
  //    2e. Nullify "solid cell" velocities.
  // 3. Move visible particles through the velocity field.

  // TODO, create Fluid class to encapsulate Grid and Particles.
  Grid2D<float> grid(128, 128);
  
  return 0;
}
