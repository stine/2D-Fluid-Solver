#include <QtGui/QApplication>
#include <vector>
#include <algorithm>
#include "MainWindow.h"
#include "FluidSolver.h"
#include "CompatibilityRenderer.h"

using namespace std;


// TODO - YUCK - This global variable is a temporary hack!!!
FluidSolver *solver = NULL;


int main(int argc, char *argv[])
{
  // Create the Qt application.
  QApplication app(argc, argv);

  // Load the simulation start state.
  vector<bool> initialFluid(16*16, false);
  fill(initialFluid.end()-16, initialFluid.end(), true);

  // Instantiate the Fluid Solver.
  solver = new FluidSolver(16, 16, initialFluid);
  
  // Create and realize UI widgets.
  MainWindow window;
  window.resize(window.sizeHint());
  window.show();

  // ...timer...?

  // Begin the Qt event loop.
  return app.exec();
}
