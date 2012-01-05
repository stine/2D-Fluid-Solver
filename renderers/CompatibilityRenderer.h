#ifndef __COMPATIBILITY_RENDERER_H__
#define __COMPATIBILITY_RENDERER_H__

#include <QGLWidget>
#include "FluidRenderer.h"
#include "FluidSolver.h"
#include "Grid.h"

class CompatibilityRenderer : public FluidRenderer
{
  const FluidSolver &_solver;

public:
  // Constructs a renderer that specifically handles 2D fluid simulations
  // and performs all rendering using immediate-mode OpenGL commands.
  // This class serves as a self-contained Qt widget and OpenGL context
  // wrapper.
  //
  // Arguments:
  //   FluidSolver &solver - Const reference to a fluid solver object.
  //   QWidget *parent - Optional pointer to this widget's parent.
  CompatibilityRenderer(const FluidSolver &solver,
			QWidget *parent = NULL);

  // Destructor
  //
  // Arguments:
  //   None
  virtual ~CompatibilityRenderer();

  // Renders the fluid simulation grid, the contents of each cell
  // (liquid, solid, or gas), and velocity vectors at the center of each
  // cell.
  //
  // Arguments:
  //   Grid &grid - The grid object containing all simulation cell data.
  //
  // Returns:
  //   None
  virtual void drawGrid(const Grid &grid);

  // Provides the required OpenGL context arguments for this renderer.
  // 
  // Arguments:
  //   None
  //
  // Returns:
  //   QGLFormat - The OpenGL context format required by this renderer.
  static QGLFormat getFormat();

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
  virtual void initializeGL();

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
  virtual void resizeGL(int width, int height);

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
  virtual void paintGL();
};

#endif // __COMPATIBILITY_RENDERER_H__
