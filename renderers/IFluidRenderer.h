#ifndef __I_FLUID_RENDERER_H__
#define __I_FLUID_RENDERER_H__

#include "GL3/gl3.h"
#include <QGLFormat>
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

  // TODO document
  virtual void beginFrame() = 0;

  // TODO document
  virtual void endFrame() = 0;

  // Renders the outlines of the MAC grid's cells.
  //
  // Arguments:
  //   Grid &grid - The grid object containing all simulation cell data.
  //
  // Returns:
  //   None
  virtual void drawGrid(const Grid &grid) = 0;

  // Renders the filled area of the MAC grid's cells.
  //
  // Arguments:
  //   Grid &grid - The grid object containing all simulation cell data.
  //
  // Returns:
  //   None
  virtual void drawCells(const Grid &grid) = 0;

  // Renders the velocity vectors showing fluid flow.
  //
  // Arguments:
  //   Grid &grid - The grid object containing all simulation cell data.
  //
  // Returns:
  //   None
  virtual void drawVectors(const Grid &grid) = 0;

  // Renders the fluid simulation's particles.
  //
  // Arguments:
  //   vector<Vector2> &particles - The particles visually representing fluid.
  //
  // Returns:
  //   None
  virtual void drawParticles(const std::vector<Vector2> &particles) = 0;
};

#endif // __FLUID_RENDERER_H__
