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
  float width = 3.0f;
  float height = 3.0f;
  Grid grid(width, height);
  for (float y = 0; y < height; ++y) 
    for (float x = 0; x < width; ++x) {
      grid(x,y).cellType = Cell::FLUID;
      grid(x,y).pressure = 1.0f;
      grid(x,y).vel[Cell::X] = sin(x * 45.215 + y * 88.15468) / 2; // arbitrary constants
      grid(x,y).vel[Cell::Y] = sin(x * 2.548 + y * 121.1215) / 2;  // arbitrary constants
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
