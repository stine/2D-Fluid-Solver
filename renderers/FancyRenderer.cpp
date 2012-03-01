#include "GL3/gl3.h"
#include "FancyRenderer.h"
#include <cstdio>
#include <vector>
#include <string>
#include <cstdio>
#include "glsw.h"

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
    _gridProgram(0),
    _cellProgram(0),
    _vectorProgram(0),
    _particleProgram(0),
    _mvMatrix(1.0f),
    _mvpMatrix(1.0f)
{
}


FancyRenderer::~FancyRenderer()
{
  glDeleteBuffers(1, &_macGridBuffer);
}


GLuint FancyRenderer::initShader(string effectName)
{
  GLuint program;
  string shaderString;
  vector<GLuint> objects;

  glswInit();
  
  // Load shader program text.
  glswSetPath("./", ".glsl");
  const char *vs = glswGetShader((effectName + ".Vertex").c_str());
  if (!vs) {
    fprintf(stderr, "%s\n", glswGetError());
    glswShutdown();
    return 0;
  }
  const char *fs = glswGetShader((effectName + ".Fragment").c_str());
  if (!fs) {
    fprintf(stderr, "%s\n", glswGetError());
    glswShutdown();
    return 0;
  }

  // Compile shader programs.
  objects.push_back(shaderObj(GL_VERTEX_SHADER, vs));
  objects.push_back(shaderObj(GL_FRAGMENT_SHADER, fs));

  // Link shader program.
  program = shaderProgram(objects);

  glswShutdown();

  return program;
}


void FancyRenderer::initialize()
{
  // Basic OpenGL setup.
  glEnable(GL_DEPTH_TEST);
  glPointSize(2.0f);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  // Load all shader programs.
  _gridProgram = initShader("Grid");
  _cellProgram = initShader("Cells");
  _vectorProgram = initShader("Vectors");
  _particleProgram = initShader("Particles");

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


void FancyRenderer::drawGrid(const Grid &grid, 
			     const vector<Vector2> &particles)
{
  // Get grid dimensions.
  float height = grid.getRowCount();
  float width  = grid.getColCount();

  // Clear the existing framebuffer contents.
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  // TODO: Draw the vertical and horizontal grid lines.
  float gridColor[4] = {1.0f, 0.0f, 0.0f, 1.0f};
  glUniform4fv(glGetUniformLocation(_gridProgram, "color"), 1, gridColor);
  glUniformMatrix4fv(glGetUniformLocation(_gridProgram, "mvpMatrix"),
		     1, false, &_mvpMatrix[0][0]);
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

  // Color the cells gray if they currently contain liquid.
  glColor4f(1.0f, 1.0f, 1.0f, 0.1f);
  glDepthMask(GL_FALSE);
  for (unsigned y = 0; y < height; ++y) {
    for (unsigned x = 0; x < width; ++x) {
      if (grid(x, y).cellType == Cell::FLUID) {
	glBegin(GL_TRIANGLES);
	glVertex2f(x, y);
	glVertex2f(x+1, y);
	glVertex2f(x+1, y+1);
	glVertex2f(x+1, y+1);
	glVertex2f(x, y+1);
	glVertex2f(x, y);
	glEnd();
      }
    }
  }

  // Draw the velocity vector at the center of each cell.
  glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
  for (float y = 0.5f; y < grid.getHeight(); y += 1.0f) {
    for (float x = 0.5f; x < grid.getWidth(); x += 1.0f) {
      Vector2 vec = grid.getVelocity(Vector2(x, y)) * 0.5;
      glBegin(GL_LINES);
      glVertex2f(x, y);
      glVertex2f(x + vec.x, y + vec.y);
      glEnd();
    }
  }

  // Draw the points.
  glColor4f(0.0f, 0.6f, 0.8f, 1.0f);
  glBegin(GL_POINTS);
  vector<Vector2>::const_iterator itr = particles.begin();
  for(; itr != particles.end(); ++itr)
  {
    glVertex2f(itr->x, itr->y);
  }
  glEnd();
}
