#ifndef __FLUID_RENDERER_H__
#define __FLUID_RENDERER_H__

#include <QGLWidget>
#include "Grid.h"

class FluidRenderer : public QGLWidget
{
  Q_OBJECT

public:
  // Constructs a renderer for a fluid simulation. This serves as a Qt widget,
  // and also contains all necessary logic to render the fluid simulation,
  // when specialized.
  // 
  // Arguments:
  //   QGLFormat &format - The requested format for the OpenGL context.
  //   QWidget *parent - The (optional) parent widget.
  FluidRenderer(const QGLFormat &format,
		QWidget *parent = NULL);

  // Destructor
  //
  // Arguments:
  //   None
  virtual ~FluidRenderer();

  // Returns the recommended minimum size for this widget.
  // 
  // Inherited from QGLWidget.
  //
  // Arguments:
  //   None
  //
  // Returns:
  //   QSize - The recommended minimum size for this widget.
  virtual QSize minimumSizeHint() const;

  // Returns the recommended size for this widget.
  //
  // Inherited from QGLWidget.
  // 
  // Arguments:
  //   None
  //
  // Returns:
  //   QSize - The recommended size for this widget.
  virtual QSize sizeHint() const;

  // Renders the fluid simulation grid, the contents of each cell
  // (liquid, solid, or gas), and velocity vectors at the center of each
  // cell.
  //
  // Arguments:
  //   Grid2D &grid - The grid object containing all simulation cell data.
  //
  // Returns:
  //   None
  virtual void drawGrid2D(const Grid2D &grid) = 0;

protected:
  // Performs all initial OpenGL commands, including modelview and projection
  // matrix initial setup, geometry loading, etc.  Called once before the first
  // call to resizeGL or paintGL.
  //
  // Inherited from QGLWidget.
  //
  // Arguments:
  //   None
  //
  // Returns:
  //   None
  virtual void initializeGL() = 0;

  // Performs necessary logic to resize the widget.  Typically involves
  // redefining the viewport and projection matrix.  Called whenever 
  // the widget changes size.
  //
  // Inherited from QGLWidget.
  //
  // Arguments:
  //   int width - The new width of the widget, in pixels.
  //   int height - The new height of the widget, in pixels.
  //
  // Returns:
  //   None
  virtual void resizeGL(int width, int height) = 0;

  // Performs all rendering for the widget.  Called whenever the widget
  // needs to be redrawn. Typically initiated by a call to updateGL.
  //
  // Inherited from QGLWidget.
  //
  // Arguments:
  //   None
  //
  // Returns:
  //   None
  virtual void paintGL() = 0;
};

#endif // __FLUID_RENDERER_H__
