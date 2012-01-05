#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QtGui>
#include <QWidget>
#include "QRendererWidget.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QWidget
{
  Q_OBJECT
  QRendererWidget *_rendWidget;
  QHBoxLayout     *_mainLayout;
    
public:
  MainWindow();
  virtual ~MainWindow();
};

#endif // __MAINWINDOW_H__
