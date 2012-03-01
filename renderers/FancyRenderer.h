#ifndef __FANCY_RENDERER_H__
#define __FANCY_RENDERER_H__

#include <QGLWidget>
#include <vector>
#include <string>
#include "IFluidRenderer.h"
#include "Grid.h"
#include "GL3/gl3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_access.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform2.hpp"


class FancyRenderer : public IFluidRenderer
{
private:
  GLuint    _macGridBuffer;
  GLuint    _gridProgram;
  GLuint    _cellProgram;
  GLuint    _vectorProgram;
  GLuint    _particleProgram;  
  glm::mat4 _mvMatrix;
  glm::mat4 _mvpMatrix;  // Projection * ModelView matrix
  float     _gridColor[4];

public:
  // Constructor
  FancyRenderer();

  // Destructor
  //
  // Arguments:
  //   None
  virtual ~FancyRenderer();

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

protected:
  // Creates a vertex buffer object representing the MAC grid lines.
  //
  // Arguments:
  //   float cellWidth - The width of a single MAC cell.
  //   unsigned cellXCount - The number of cells in the sim; X dimension.
  //   unsigned cellYCount - The number of cells in the sim; Y dimension.
  //
  // Returns:
  //   GLuint - The handle to the generated buffer object.
  GLuint genMacGridBuffer(float cellWidth,
			  unsigned cellXCount,
			  unsigned cellYCount);

  // Creates a vertex buffer object representing the MAC grid cells.
  //
  // Arguments:
  //   float cellWidth - The width of a single MAC cell.
  //   unsigned cellXCount - The number of cells in the sim; X dimension.
  //   unsigned cellYCount - The number of cells in the sim; Y dimension.
  //
  // Returns:
  //   GLuint - The handle to the generated buffer object.
  GLuint genCellBuffer(float cellWidth,
		       unsigned cellXCount,
		       unsigned cellYCount);

  // TODO documentation.
  GLuint initShader(std::string effectName);
};

#endif // __FANCY_RENDERER_H__
