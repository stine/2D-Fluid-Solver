#include <QtGui>
#include "MainWindow.h"
#include "QRendererWidget.h"
#include "SignalRelay.h"

MainWindow::MainWindow()
  : _mainLayout(NULL),
    _controlLayout(NULL),
    _rendWidget(NULL),
    _resetButton(NULL)
    
{
  // Establish a default renderer to use.
  _rendWidget = QRendererWidget::rendererWidget
    (this, QRendererWidget::FANCY_RENDERER);

  // Create the overall window layout - an HBoxLayout.
  _mainLayout = new QHBoxLayout;
  
  // Two children of the HBoxLayout: renderer and VBoxLayout (controls).
  _mainLayout->addWidget(_rendWidget);  
  _controlLayout = new QVBoxLayout;
  _mainLayout->addLayout(_controlLayout);

  // Children of the VBoxLayout.
  _resetButton = new QPushButton("Reset");
  _controlLayout->addWidget(_resetButton);
  QObject::connect(_resetButton, SIGNAL(clicked()),
		   SignalRelay::getInstance(), SIGNAL(resetSimulation()));

  // Realize all widgets.
  setLayout(_mainLayout);

  // Set window title.
  setWindowTitle(tr("2D Fluid Solver"));
}

MainWindow::~MainWindow()
{
  // Clean up memory.
  delete _rendWidget;
  delete _resetButton;
  delete _controlLayout;
  delete _mainLayout;
}
