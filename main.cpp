#include <QtGui/QApplication>
#include <vector>
#include "MainWindow.h"
#include "FluidSolver.h"
#include "FluidRenderer2D.h"

using namespace std;

int main(int argc, char *argv[])
{
  // Create the Qt application.
  QApplication app(argc, argv);

  // Load the simulation start state.
  vector<bool> initialFluid;    // TODO load from file.

  // Instantiate the Fluid Solver.
  FluidSolver2D solver(64, 64, initialFluid);
  
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
