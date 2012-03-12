#ifndef __FLUID_SOLVER_H__
#define __FLUID_SOLVER_H__

#include "Grid.h"
#include "Vector2.h"
#include "IFluidRenderer.h"
#include <vector>


class FluidSolver : public QObject
{
  Q_OBJECT

private:
  const float     _width;       // The width of the simulation.
  const float     _height;      // The height of the simulation.
  Grid            _grid;        // The 2D MAC Grid.
  Vector2 _maxVelocity; // The maximum velocity seen last timestep.
  bool            _frameReady;  // True if frame's calculations are complete.
  std::vector<Vector2> _particles;

public:
  // Constructs a 2D fluid simulation of the specified size.
  // Currently each cell is 1.0f units by 1.0f units.
  //
  // Arguments:
  //   float width - The width of the simulation, in world coordinates.
  //   float height - The height of the simulation, in world coordinates.
  FluidSolver(float width, float height);

  // Destructs the solver.
  virtual ~FluidSolver();

  // Draws the current state of the simulation using the provided FluidRenderer.
  // 
  // Arguments:
  //   IFluidRenderer *renderer - The FluidRenderer that will draw all sim data.
  //
  // Returns:
  //   None
  void draw(IFluidRenderer *renderer);
  
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

public slots:
  // Advances the simulation by a single frame if necessary.  If a frame has
  // already been calculated but not yet drawn (by calling the draw() method
  // of this class), this method immediately returns.
  // Calculating a single frame involves determining an appropriate timestep
  // based on the CFL condition, and potentially advancing the simulation
  // multiple times based on that timestep until the simulation over the
  // anticipated duration of the frame has been calculated.
  //
  // Arguments:
  //   None
  //
  // Returns:
  //   None
  void advanceFrame();

  // Resets the simulation to a default starting grid.
  // This is primarily to be used for debugging during development, and is
  // expected to change regularly as more capabilities are added.
  //
  // Arguments:
  //   None
  //
  // Returns:
  //   None
  void reset();

protected:
  // Advances the simulation by a specific amount of time.
  //
  // Arguments:
  //   float timeStepSec - The amount of time to simulate.
  //
  // Returns:
  //   None
  void advanceTimeStep(float timeStepSec);

  // Advects the fluid's velocity field via a backward particle trace,
  // over the specified amount of time.
  //
  // Arguments:
  //   float timeStepSec - The amount of time to advect over.
  //
  // Returns:
  //   None
  void advectVelocity(float timeStepSec);
  
  // Applies a global velocity to all cells containing fluid. This is helpful
  // for simulating gravity.
  //
  // Arguments:
  //   Vector<2,float> velocity - The velocity to apply to all fluid cells.
  // 
  // Returns:
  //   None
  void applyGlobalVelocity(Vector2 velocity);
  
  // Adjusts velocity field based on the pressure scalar field to enforce 
  // incompressibility (non-divergence) of the fluid, and redistributes
  // pressure values appropriately.
  //
  // Arguments:
  //   float timeStepSec - The amount of time to simulate.
  // 
  // Returns:
  //   None
  void pressureSolve(float timeStepSec);
  
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

  // Moves particles through the velocity field for the specified duration.
  //
  // Arguments:
  //   float timeStepSec - The duration to move the particles through the fluid.
  // 
  // Returns:
  //   None
  void moveParticles(float timeStepSec);

private:
  // Hidden default constructor.
  FluidSolver();
};

#endif //__FLUID_SOLVER_H__
