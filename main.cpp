#include <QtGui/QApplication>
#include <vector>
#include <algorithm>
#include "MainWindow.h"
#include "FluidSolver.h"
#include "FluidRenderer2D.h"

using namespace std;

int main(int argc, char *argv[])
{
  // Create the Qt application.
  QApplication app(argc, argv);

  // Load the simulation start state.
  // TODO load from file, currently sets top row to "true".
  vector<bool> initialFluid(16*16, false);
  fill(initialFluid.end()-16, initialFluid.end(), true);

  // Instantiate the Fluid Solver.
  FluidSolver2D solver(16, 16, initialFluid);
  
  // Instantiate the OpenGL renderer.
  FluidRenderer2D renderer(&solver);

  // Create and realize UI widgets.
  MainWindow window(&renderer);
  window.resize(window.sizeHint());
  window.show();

  // ...timer...?

  // Begin the Qt event loop.
  return app.exec();
}
