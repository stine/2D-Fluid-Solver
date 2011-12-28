#ifndef __FLUID_RENDERER_2D_H__
#define __FLUID_RENDERER_2D_H__

#include <QGLWidget>
#include "FluidRenderer.h"
#include "FluidSolver.h"
#include "Grid.h"

class FluidRenderer2D : public FluidRenderer
{
  const FluidSolver2D *_solver;

public:
  // Constructs a renderer that specifically handles 2D fluid simulations.
  // This class serves as a self-contained Qt widget and OpenGL context
  // wrapper.
  //
  // Arguments:
  //   FluidSolver2D *solver - Pointer to a fluid solver object.
  //   QWidget *parent - Optional pointer to this widget's parent.
  FluidRenderer2D(const FluidSolver2D *solver,
		  QWidget *parent = NULL);

  // Destructor
  //
  // Arguments:
  //   None
  virtual ~FluidRenderer2D();

  // Renders the fluid simulation grid, the contents of each cell
  // (liquid, solid, or gas), and velocity vectors at the center of each
  // cell.
  //
  // Arguments:
  //   Grid2D &grid - The grid object containing all simulation cell data.
  //
  // Returns:
  //   None
  virtual void drawGrid2D(const Grid2D &grid);

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

#endif // __FLUID_RENDERER_2D_H__
