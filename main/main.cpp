#include <QtGui/QApplication>
#include <QTimer>
#include <vector>
#include <algorithm>
#include "MainWindow.h"
#include "FluidSolver.h"

using namespace std;


// TODO - YUCK - This global variable is a temporary hack!!!
FluidSolver *solver = NULL;

int main(int argc, char *argv[])
{
  // Create the Qt application.
  QApplication app(argc, argv);

  // Instantiate the Fluid Solver using the initial velocity field.
  solver = new FluidSolver(8.0f, 8.0f);
  
  // Create and realize UI widgets.
  MainWindow window;
  window.resize(window.sizeHint());
  window.show();

  // Kick off a timer to calculate new frames.
  QTimer *timer = new QTimer(solver);
  QObject::connect(timer, SIGNAL(timeout()), solver, SLOT(advanceFrame()));
  timer->start();

  // Begin the Qt event loop.
  return app.exec();
}
