#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QtGui>
#include <QWidget>
#include "FluidRenderer.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QWidget
{
  Q_OBJECT
  FluidRenderer *_renderer;
  QHBoxLayout *_mainLayout;
    
public:
  MainWindow(FluidRenderer *renderer);
  virtual ~MainWindow();
};

#endif // __MAINWINDOW_H__
