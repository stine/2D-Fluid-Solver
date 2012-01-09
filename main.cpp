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

  // Initialize a velocity field for testing.
  // Note: values in this velocity field are arbitrarily chosen and may be
  //  divergent within a cell.
  // Note: sin() is used to clamp output values to [-1, 1].
  const unsigned rows = 3;
  const unsigned cols = 3;
  const unsigned size = rows * cols;
  Grid grid(3, 3);
  for (unsigned i = 0; i < size; ++i) {
    grid[i].isLiquid = true;
    grid[i].pressure = 1.0f;
    grid[i].vel[Cell::X] = sin(i * 45.215 + 88.15468) / 2; // arbitrary constants
    grid[i].vel[Cell::Y] = sin(i * 2.548 + 121.1215) / 2;  // arbitrary constants
  }

  // Instantiate the Fluid Solver using the initial velocity field.
  solver = new FluidSolver(grid);
  
  // Create and realize UI widgets.
  MainWindow window;
  window.resize(window.sizeHint());
  window.show();

  // ...timer...?

  // Begin the Qt event loop.
  return app.exec();
}
