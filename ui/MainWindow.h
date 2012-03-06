#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include "GL3/gl3.h"
#include <QtGui>
#include <QWidget>
#include "QRendererWidget.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QWidget
{
  Q_OBJECT
  QHBoxLayout     *_mainLayout;
  QVBoxLayout     *_controlLayout;
  QRendererWidget *_rendWidget;
  QPushButton     *_resetButton;
    
public:
  MainWindow();
  virtual ~MainWindow();
};

#endif // __MAINWINDOW_H__
