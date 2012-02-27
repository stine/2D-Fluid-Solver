#ifndef __I_FLUID_RENDERER_H__
#define __I_FLUID_RENDERER_H__

#include <QGLWidget>
#include <vector>
#include "Grid.h"

class IFluidRenderer
{
public:
  // Destructor
  //
  // Arguments:
  //   None
  virtual ~IFluidRenderer() {}

  // Provides the required OpenGL context arguments for this renderer.
  // 
  // Arguments:
  //   None
  //
  // Returns:
  //   QGLFormat - The OpenGL context format required by this renderer.
  virtual QGLFormat getFormat() = 0;

  // Performs all initial OpenGL commands, including modelview and projection
  // matrix initial setup, geometry loading, etc.  Called once before the first
  // call to resizeGL or paintGL.
  //
  // Arguments:
  //   None
  //
  // Returns:
  //   None
  virtual void initialize() = 0;

  // Performs necessary logic to resize the drawing area.  Typically involves
  // redefining the viewport and projection matrix.  Called whenever the widget
  // changes size.
  //
  // Arguments:
  //   int pixWidth - The new width of the widget, in pixels.
  //   int pixHeight - The new height of the widget, in pixels.
  //
  // Returns:
  //   None
  virtual void resize(int pixWidth, int pixHeight) = 0;

  // Renders the fluid simulation grid.
  //
  // Arguments:
  //   Grid &grid - The grid object containing all simulation cell data.
  //   vector<Vector2> &particles - The particles visually representing fluid.
  //
  // Returns:
  //   None
  virtual void drawGrid(const Grid &grid, 
                        const std::vector<Vector2> &particles) = 0;
};

#endif // __FLUID_RENDERER_H__
