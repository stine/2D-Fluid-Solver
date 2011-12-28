#include <QtGui>
#include "MainWindow.h"
#include "FluidRenderer.h"

MainWindow::MainWindow(FluidRenderer *renderer)
  : _renderer(renderer),
    _mainLayout(NULL)
{
  // Connect the widgets to the widget heirarchy.
  _mainLayout = new QHBoxLayout;
  _mainLayout->addWidget(renderer);  
  _renderer->setParent(this);
  setLayout(_mainLayout);

  // Set window title.
  setWindowTitle(tr("2D Fluid Solver"));
}

MainWindow::~MainWindow()
{
  // Clean up memory.
  delete _mainLayout;
  _mainLayout = NULL;
}
