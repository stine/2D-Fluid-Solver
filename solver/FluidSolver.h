#ifndef __FLUID_SOLVER_H__
#define __FLUID_SOLVER_H__

#include "Grid.h"
#include "Vector.h"
#include "IFluidRenderer.h"


class FluidSolver : public QObject
{
  const float     _width;       // The width of the simulation.
  const float     _height;      // The height of the simulation.
  Grid            _grid;        // The 2D MAC Grid.
  Vector<2,float> _maxVelocity; // The maximum velocity seen last timestep.
  // TODO vector<MarkerParticle>

public:
  // Constructs a 2D fluid simulation around the specified initial grid.  Currently
  // each cell is 1.0f units by 1.0f units in size.
  //
  // Arguments:
  //   Grid &grid - The initial velocity field to use for the fluid simulation.
  FluidSolver(const Grid &grid);

  // Advances the simulation by an appropriately-sized timestep. The timestep
  // is calculated based on the maximum velocity seen in the simulation at the
  // end of the previous timestep.
  //
  // Arguments:
  //   None
  //
  // Returns:
  //   float - The number of seconds simulated.
  float advanceSimulation();

  // Draws the current state of the simulation using the provided FluidRenderer.
  // 
  // Arguments:
  //   IFluidRenderer *renderer - The FluidRenderer to draw all sim data.
  //
  // Returns:
  //   None
  void draw(IFluidRenderer *renderer) const;
  
  // Returns the simulation width.
  //
  // Arguments:
  //   None
  // 
  // Returns:
  //   float - The width of the simulation.
  float getSimulationWidth() const;
  
  // Returns the simulation height.
  //
  // Arguments:
  //   None
  // 
  // Returns:
  //   float - The height of the simulation.
  float getSimulationHeight() const;

protected:
  // Advects the fluid's velocity field via a backward particle trace.
  //
  // Arguments:
  //   None
  //
  // Returns:
  //   None
  void advectVelocity();
  
  // Applies a global force to all cells containing fluid. This is helpful
  // for simulating gravity.
  //
  // Arguments:
  //   Vector<2,float> force - The global force to apply to all fluid cells.
  // 
  // Returns:
  //   None
  void applyGlobalForce(Vector<2,float> force);
  
  // Solves the system of linear equations that enforce incompressibility
  // of the fluid.  This ensures divergence of the velocity field at each
  // cell is zero.
  //
  // Arguments:
  //   None
  // 
  // Returns:
  //   None
  void pressureSolve();
  
  // Modifies velocity values to prevent the fluid from flowing out of the
  // simulation boundaries.
  //
  // TODO:
  //   Does this cover the case where fluid cells travel into air?
  //
  // Arguments:
  //   None
  //
  // Returns:
  //   None
  void boundaryCollide();

private:
  // Hidden default constructor.
  FluidSolver();
};

#endif //__FLUID_SOLVER_H__
