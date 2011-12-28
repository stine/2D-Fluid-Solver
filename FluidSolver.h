#ifndef __FLUID_SOLVER_H__
#define __FLUID_SOLVER_H__

#include "Grid.h"
#include "Vector.h"

class FluidRenderer2D;

class FluidSolver2D
{
  const unsigned  _iSize;       // The number of rows in the sim.
  const unsigned  _jSize;       // The number of columns in the sim.
  Grid2D          _grid;        // The 2D MAC Grid.
  Vector<2,float> _maxVelocity; // The maximum velocity seen last timestep.
  // TODO vector<MarkerParticle>

public:
  // Constructs a 2D fluid simulation of the specified size.  Currently
  // each cell is 1.0f units by 1.0f units in size.
  //
  // Arguments:
  //   unsigned iSize - The number of rows of cells in the simulation.
  //   unsigned jSize - The number of columns of cells in the simulation.
  //   vector<bool> initialFluid - Array of flags representing initial state.
  FluidSolver2D
    (unsigned iSize,
     unsigned jSize,
     const std::vector<bool> &initialFluid);

  // Advances the simulation by an appropriately-sized timestep. The timestep
  // is calculated based on the maximum velocity seen in the simulation at the
  // end of the previous timestep.
  //
  // Arguments:
  //   None
  // Returns:
  //   float - The number of seconds simulated.
  float advanceSimulation();

  // Draws the current state of the simulation using the provided FluidRenderer.
  // 
  // Arguments:
  //   FluidRenderer2D *renderer - The FluidRenderer2D to draw all sim data.
  // Returns:
  //   None
  void draw(FluidRenderer2D *renderer) const;

protected:
  // Advects the fluid's velocity field via a backward particle trace.
  //
  // Arguments:
  //   None
  // Returns:
  //   None
  void advectVelocity();
  
  // Applies a global force to all cells containing fluid. This is helpful
  // for simulating gravity.
  //
  // Arguments:
  //   Vector<2,float> force - The global force to apply to all fluid cells.
  // Returns:
  //   None
  void applyGlobalForce(Vector<2,float> force);
  
  // Solves the system of linear equations that enforce incompressibility
  // of the fluid.  This ensures divergence of the velocity field at each
  // cell is zero.
  //
  // Arguments:
  //   None
  // Returns:
  //   None
  void pressureSolve();
  
  // Modifies velocity values to prevent the fluid from flowing out of the
  // simulation boundaries.
  //
  // TODO:
  //   Does this cover the case where fluid cells travel into air?
  // Arguments:
  //   None
  // Returns:
  //   None
  void boundaryCollide();

private:
  // Hidden default constructor.
  FluidSolver2D();
};

#endif //__FLUID_SOLVER_H__
