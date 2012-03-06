#ifndef __FANCY_RENDERER_H__
#define __FANCY_RENDERER_H__

#include "GL3/gl3.h"
#include <QGLWidget>
#include <vector>
#include <string>
#include "IFluidRenderer.h"
#include "Grid.h"
#include "Shader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_access.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform2.hpp"


class FancyRenderer : public IFluidRenderer
{
private:
  // Related to drawing MAC cell backgrounds
  GLuint   _cellVAO;
  GLuint   _cellVBO;
  bool     _isCellVBO;
  unsigned _cellCount;
  Shader   _cellProgram;
  
  // Related to drawing MAC cell grid lines
  GLuint   _gridVAO;
  GLuint   _gridVBO;
  bool     _isGridVBO;
  unsigned _lineVertCnt;
  Shader   _gridProgram;

  // Related to drawing velocity vectors
  GLuint _velocityVAO;
  GLuint _cellCenterPtsVBO;
  GLuint _velocityVecsVBO;
  bool   _isCellCenterPtsVBO;
  Shader _velocityProgram;

  // Related to drawing particles
  GLuint _particlesVAO;
  GLuint _particlesVBO;
  Shader _particleProgram;
  
  // Matrices
  glm::mat4 _mvMatrix;
  glm::mat4 _mvpMatrix;  // Projection * ModelView matrix

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

protected:
  // Creates a VBO
  // TODO documentation
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

  // Creates a VBO
  // TODO documentation
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
