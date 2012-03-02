#include "GL3/gl3.h"
#include <cstdio>
#include <vector>
#include <string>
#include <cstdio>
#include "FancyRenderer.h"
#include "Shader.h"

// TODO - YUCK - This global variable is a temporary hack!!!
#include "FluidSolver.h"
extern FluidSolver *solver;

using std::vector;
using std::string;

QGLFormat FancyRenderer::getFormat()
{
  // Specify the necessary OpenGL context attributes for this renderer.
  QGLFormat glFormat;
  glFormat.setVersion(3, 0);
  glFormat.setSwapInterval(1);
  return glFormat;
}


FancyRenderer::FancyRenderer()
  : _macGridBuffer(0),
    _mvMatrix(1.0f),
    _mvpMatrix(1.0f)
{
}


FancyRenderer::~FancyRenderer()
{
  glDeleteBuffers(1, &_macGridBuffer);
}


void FancyRenderer::initialize()
{
  // Basic OpenGL setup.
  glEnable(GL_DEPTH_TEST);
  glPointSize(2.0f);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  // Load all shader programs.
  _gridProgram.compile("Grid.Vertex", NULL, "Grid.Fragment");
  _cellProgram.compile("Cells.Vertex", NULL, "Cells.Fragment");
  _vectorProgram.compile("Vectors.Vertex", "Vectors.Geometry", "Vectors.Fragment");
  _particleProgram.compile("Particles.Vertex", NULL, "Particles.Fragment");

  // Set default modelview matrix.
  _mvMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, -10));
}


void FancyRenderer::resize(int pixWidth, int pixHeight)
{
  // Define a viewport based on widget size.
  glViewport(0, 0, pixWidth, pixHeight);

  // For the purpose of fitting the grid within the rendering area, take into
  // account a margin of 1 cell around the grid.
  unsigned rawWidth  = solver->getSimulationWidth();
  unsigned rawHeight = solver->getSimulationHeight();
  unsigned paddedWidth  = rawWidth + 2;
  unsigned paddedHeight = rawHeight + 2;

  // Set default values for the projection matrix.  Directly using these values
  // will result in the grid perfectly fitting within the rendering area, but
  // the grid will be stretched according to the aspect ratio.
  float xMin = -1.0f;
  float xMax = xMin + paddedWidth;
  float yMin = -1.0;
  float yMax = yMin + paddedHeight;
  
  // Calculate the ratio of rendering area pixels per cell along each axis.
  float pixPerCellW = float(pixWidth) / float(paddedWidth);
  float pixPerCellH = float(pixHeight) / float(paddedHeight);
  if (pixPerCellW < pixPerCellH) {
    // Width is the dominant dimension.
    float simHeight = float(paddedWidth) * float(pixHeight) / float(pixWidth);
    yMin = -(simHeight - rawHeight) / 2;
    yMax = yMin + simHeight;
  }
  else {
    // Height is the dominant dimension.
    float simWidth = float(paddedHeight) * float(pixWidth) / float(pixHeight);
    xMin = -(simWidth - rawHeight) / 2;
    xMax = xMin + simWidth;
  }
  
  // Define projection  matrix based on widget size.  Multiply by modelview matrix.
  _mvpMatrix = glm::ortho(xMin, xMax, yMin, yMax, 5.0f, 15.0f) * _mvMatrix;
}


void FancyRenderer::beginFrame()
{
  // Clear the existing framebuffer contents.
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void FancyRenderer::endFrame()
{}


void FancyRenderer::drawGrid(const Grid &grid)
{
  // Get grid dimensions.
  float height = grid.getRowCount();
  float width  = grid.getColCount();

  // TODO: Draw the vertical and horizontal grid lines.
  float gridColor[4] = {1.0f, 0.0f, 0.0f, 1.0f};
  glUniformMatrix4fv(glGetUniformLocation(_gridProgram, "mvpMatrix"),
		     1, false, &_mvpMatrix[0][0]);
  glUniform4fv(glGetUniformLocation(_gridProgram, "color"), 1, gridColor);
  glBindFragDataLocation(_gridProgram, 0, "fragcolor");
  glBegin(GL_LINES);
  for (unsigned i = 0; i <= width; ++i) {
    glVertex2f(i, 0);
    glVertex2f(i, height);
  }
  for (unsigned i = 0; i <= height; ++i) {
    glVertex2f(0, i);
    glVertex2f(width, i);
  }
  glEnd();
}


void FancyRenderer::drawCells(const Grid &grid)
{

}


void FancyRenderer::drawVectors(const Grid &grid)
{

}


void FancyRenderer::drawParticles(const std::vector<Vector2> &particles)
{

}
