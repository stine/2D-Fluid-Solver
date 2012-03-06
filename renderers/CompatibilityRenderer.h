#ifndef __COMPATIBILITY_RENDERER_H__
#define __COMPATIBILITY_RENDERER_H__

#include "GL3/gl3.h"
#include <QGLWidget>
#include <vector>
#include "IFluidRenderer.h"
#include "Grid.h"


class CompatibilityRenderer : public IFluidRenderer
{
public:
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

  // TODO document
  virtual void beginFrame();

  // TODO document
  virtual void endFrame();

  // Renders the outlines of the MAC grid's cells.
  //
  // Arguments:
  //   Grid &grid - The grid object containing all simulation cell data.
  //
  // Returns:
  //   None
  virtual void drawGrid(const Grid &grid);

  // Renders the filled area of the MAC grid's cells.
  //
  // Arguments:
  //   Grid &grid - The grid object containing all simulation cell data.
  //
  // Returns:
  //   None
  virtual void drawCells(const Grid &grid);

  // Renders the velocity vectors showing fluid flow.
  //
  // Arguments:
  //   Grid &grid - The grid object containing all simulation cell data.
  //
  // Returns:
  //   None
  virtual void drawVectors(const Grid &grid);

  // Renders the fluid simulation's particles.
  //
  // Arguments:
  //   vector<Vector2> &particles - The particles visually representing fluid.
  //
  // Returns:
  //   None
  virtual void drawParticles(const std::vector<Vector2> &particles);
};

#endif // __COMPATIBILITY_RENDERER_H__
