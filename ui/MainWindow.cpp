#include <QtGui>
#include "MainWindow.h"
#include "QRendererWidget.h"

MainWindow::MainWindow()
  : _rendWidget(NULL),
    _mainLayout(NULL)
{
  // Establish a default renderer to use.
  _rendWidget = QRendererWidget::rendererWidget
    (this, QRendererWidget::COMPATIBILITY_RENDERER);

  // Connect the widgets to the widget heirarchy.
  _mainLayout = new QHBoxLayout;
  _mainLayout->addWidget(_rendWidget);  
  setLayout(_mainLayout);

  // Set window title.
  setWindowTitle(tr("2D Fluid Solver"));
}

MainWindow::~MainWindow()
{
  // Clean up memory.
  delete _rendWidget;
  _rendWidget = NULL;
  delete _mainLayout;
  _mainLayout = NULL;
}
