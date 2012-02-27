#ifndef __COMPATIBILITY_RENDERER_H__
#define __COMPATIBILITY_RENDERER_H__

#include <QGLWidget>
#include <vector>
#include "IFluidRenderer.h"
#include "Grid.h"


class CompatibilityRenderer : public IFluidRenderer
{
public:
  // Destructor
  //
  // Arguments:
  //   None
  virtual ~CompatibilityRenderer() {}

  // Provides the required OpenGL context arguments for this renderer.
  // 
  // Arguments:
  //   None
  //
  // Returns:
  //   QGLFormat - The OpenGL context format required by this renderer.
  virtual QGLFormat getFormat();

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
  virtual void initialize();

  // Performs necessary logic to resize the widget.  Typically involves
  // redefining the viewport and projection matrix.  Called whenever 
  // the widget changes size.
  //
  // Inherited from QGLWidget.
  //
  // Arguments:
  //   int pixWidth - The new width of the widget, in pixels.
  //   int pixHeight - The new height of the widget, in pixels.
  //
  // Returns:
  //   None
  virtual void resize(int pixWidth, int pixHeight);

  // Renders the fluid simulation grid, the contents of each cell
  // (liquid, solid, or gas), and velocity vectors at the center of each
  // cell.  This also renders the particles representing the fluid.
  //
  // Should typically be called from the FluidSolver class.
  //
  // Arguments:
  //   Grid &grid - The grid object containing all simulation cell data.
  //   vector<Vector2> &particles- The particles visually representing the fluid.
  //
  // Returns:
  //   None
  virtual void drawGrid(const Grid &grid, 
                        const std::vector<Vector2> &particles);
};

#endif // __COMPATIBILITY_RENDERER_H__
