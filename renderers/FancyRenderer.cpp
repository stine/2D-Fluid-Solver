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
  glFormat.setVersion(3, 2);
  glFormat.setProfile(QGLFormat::CoreProfile);
  glFormat.setSwapInterval(1);
  return glFormat;
}


FancyRenderer::FancyRenderer()
  : _isCellVBO(false),
    _isGridVBO(false),
    _isCellCenterPtsVBO(false),
    _mvMatrix(1.0f),
    _mvpMatrix(1.0f)
{
}


FancyRenderer::~FancyRenderer()
{
  // TODO delete buffers and vertex arrays
}


void FancyRenderer::initialize()
{
  // Basic OpenGL setup.
  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_DEPTH_TEST);
  glPointSize(2.0f);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  // Load all shader programs and bind attribute locations.
  _gridProgram.compile("Grid.Vertex", NULL, "Grid.Fragment");
  glBindAttribLocation(_gridProgram, 0, "position");
  glBindFragDataLocation(_gridProgram, 0, "fragcolor");
  _gridProgram.link();
  glGenVertexArrays(1, &_gridVAO);
  glGenBuffers(1, &_gridVBO);

  _cellProgram.compile("Cells.Vertex", "Cells.Geometry", "Cells.Fragment");
  glBindAttribLocation(_cellProgram, 0, "position");
  glBindFragDataLocation(_cellProgram, 0, "fragcolor");
  _cellProgram.link();
  glGenVertexArrays(1, &_cellVAO);
  glGenBuffers(1, &_cellVBO);

  _velocityProgram.compile("Velocity.Vertex", NULL, "Velocity.Fragment");
  glBindAttribLocation(_velocityProgram, 0, "position");
  glBindAttribLocation(_velocityProgram, 1, "velocity");
  glBindFragDataLocation(_velocityProgram, 0, "fragcolor");
  _velocityProgram.link();
  glGenVertexArrays(1, &_velocityVAO);
  glGenBuffers(1, &_cellCenterPtsVBO);
  glGenBuffers(1, &_velocityVecsVBO);

  _particleProgram.compile("Particles.Vertex", NULL, "Particles.Fragment");
  glBindAttribLocation(_particleProgram, 0, "position");
  glBindFragDataLocation(_particleProgram, 0, "fragcolor");
  _particleProgram.link();
  glGenVertexArrays(1, &_particlesVAO);
  glGenBuffers(1, &_particlesVBO);

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
  
  // Define projection matrix based on widget size.  Multiply by modelview matrix.
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
  // Get generic attribute and uniform indices.
  GLuint posIdx = glGetAttribLocation(_gridProgram, "position");
  GLuint colIdx = glGetUniformLocation(_gridProgram, "color");
  GLuint mvpIdx = glGetUniformLocation(_gridProgram, "mvpMatrix");

  // Bind the grid VAO.
  glBindVertexArray(_gridVAO);

  // If the VBO with grid geometry hasn't been initialized, do it.
  if (!_isGridVBO) {
    // Load the grid geometry into a contiguous array.
    const float width = grid.getWidth();
    const float height = grid.getHeight();
    _lineVertCnt = (width + 1 + height + 1) * 2;
    GLfloat *lineVerts = new GLfloat[_lineVertCnt * 2];
    unsigned i = 0;
    for (float x = 0.0f; x <= width; x += 1.0f) {
      lineVerts[i++] = x;
      lineVerts[i++] = 0.0f;
      lineVerts[i++] = x;
      lineVerts[i++] = height;
    }
    for (float y = 0.0f; y <= height; y += 1.0f) {
      lineVerts[i++] = 0.0f;
      lineVerts[i++] = y;
      lineVerts[i++] = width;
      lineVerts[i++] = y;
    }

    // Upload grid geometry into a buffer object. Cleanup memory.
    glEnableVertexAttribArray(posIdx);
    glBindBuffer(GL_ARRAY_BUFFER, _gridVBO);
    glBufferData(GL_ARRAY_BUFFER, _lineVertCnt * 2 * sizeof(GLfloat), lineVerts, GL_STATIC_DRAW);
    glVertexAttribPointer(posIdx, 2, GL_FLOAT, GL_FALSE, 0, 0);
    delete [] lineVerts;

    // Set initialized flag to 'true'.
    _isGridVBO = true;
  }

  // Select shader program and load uniforms.
  float gridColor[4] = {0.2f, 0.2f, 0.2f, 1.0f};
  glUseProgram(_gridProgram);
  glUniform4fv(colIdx, 1, gridColor);
  glUniformMatrix4fv(mvpIdx, 1, false, &_mvpMatrix[0][0]);

  // Draw the bound VAO.
  glDepthMask(GL_FALSE);
  glDrawArrays(GL_LINES, 0, _lineVertCnt);
  glDepthMask(GL_TRUE);

  // Unbind VAO and Program.  Clean up memory.
  glBindVertexArray(0);
  glUseProgram(0);
}


void FancyRenderer::drawCells(const Grid &grid)
{
  // Get grid dimensions.
  const float width = grid.getWidth();
  const float height = grid.getHeight();

  // Get generic attribute and uniform indices.
  GLuint posIdx = glGetAttribLocation(_cellProgram, "position");
  GLuint widIdx = glGetUniformLocation(_cellProgram, "cellWidth");
  GLuint colIdx = glGetUniformLocation(_cellProgram, "color");
  GLuint mvpIdx = glGetUniformLocation(_cellProgram, "mvpMatrix");

  // Bind the cell VAO.
  glBindVertexArray(_cellVAO);

  // If the VBO with cell positions hasn't been initialized, do it.
  if (!_isCellVBO) {
    // Load the cell positions into a contiguous array.
    _cellCount = width * height;
    GLfloat *cellVerts= new GLfloat[_cellCount * 2];
    unsigned i = 0;
    for (float y = 0.5f; y < height; y += 1.0f)
      for (float x = 0.5f; x < width; x += 1.0f) {
	cellVerts[i++] = x;
	cellVerts[i++] = y;
      }

    // Upload cell positions into a buffer object. Clean up memory.
    glEnableVertexAttribArray(posIdx);
    glBindBuffer(GL_ARRAY_BUFFER, _cellVBO);
    glBufferData(GL_ARRAY_BUFFER, _cellCount * 2 * sizeof(GLfloat), cellVerts, GL_STATIC_DRAW);
    glVertexAttribPointer(posIdx, 2, GL_FLOAT, GL_FALSE, 0, 0);
    delete [] cellVerts;
    
    // Set initialized flag to 'true'.
    _isCellVBO = true;
  }

  // Select shader program and load uniforms.
  float cellColor[4] = {0.1f, 0.1f, 0.1f, 1.0f};
  glUseProgram(_cellProgram);
  glUniform1f(widIdx, 1.0f);
  glUniform4fv(colIdx, 1, cellColor);
  glUniformMatrix4fv(mvpIdx, 1, false, &_mvpMatrix[0][0]);

  // Draw the bound VAO.
  glDepthMask(GL_FALSE);
  glDrawArrays(GL_POINTS, 0, _cellCount);
  glDepthMask(GL_TRUE);

  // Unbind VAO and Program.  Clean up memory.
  glBindVertexArray(0);
  glUseProgram(0);
}


void FancyRenderer::drawVectors(const Grid &grid)
{
  // Get generic attribute and uniform indices.
  GLuint posIdx = glGetAttribLocation(_velocityProgram, "position");
  GLuint velIdx = glGetAttribLocation(_velocityProgram, "velocity");
  GLuint mvpIdx = glGetUniformLocation(_velocityProgram, "mvpMatrix");

  // Bind the velocity VAO.
  glBindVertexArray(_velocityVAO);

  // If the VBO with cell center positions hasn't been initialized, do it.
  if (!_isCellCenterPtsVBO) {
    // Load the cell midpoints into a contiguous array.
    unsigned cellCount = grid.getHeight() * grid.getWidth();
    GLfloat *middleVerts = new GLfloat[cellCount * 2];
    unsigned i = 0;
    for (float y = 0.5f; y < grid.getHeight(); y += 1.0f)
      for (float x = 0.5f; x < grid.getWidth(); x += 1.0f) {
	middleVerts[i++] = x;
	middleVerts[i++] = y;
      }

    // Upload cell midpoint data to a buffer object. Cleanup memory.
    glEnableVertexAttribArray(posIdx);
    glBindBuffer(GL_ARRAY_BUFFER, _cellCenterPtsVBO);
    glBufferData(GL_ARRAY_BUFFER, cellCount * 2 * sizeof(GLfloat), middleVerts, GL_STATIC_DRAW);
    glVertexAttribPointer(posIdx, 2, GL_FLOAT, GL_FALSE, 0, 0);
    delete [] middleVerts;

    // Set initialized flag to 'true'.
    _isCellCenterPtsVBO = true;
  }

  // Load the velocity field into a contiguous array.
  unsigned cellCount = grid.getHeight() * grid.getWidth();
  GLfloat *velField = new GLfloat[cellCount * 2];
  unsigned i = 0;
  for (float y = 0.5f; y < grid.getHeight(); y += 1.0f)
    for (float x = 0.5f; x < grid.getWidth(); x += 1.0f) {
      Vector2 vel = grid.getVelocity(Vector2(x, y));
      velField[i++] = vel.x;
      velField[i++] = vel.y;
    }

  // Upload the velocity field data into a buffer object.  Clean up memory.
  glEnableVertexAttribArray(velIdx);
  glBindBuffer(GL_ARRAY_BUFFER, _velocityVecsVBO);
  glBufferData(GL_ARRAY_BUFFER, cellCount * 2 * sizeof(GLfloat), velField, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(velIdx, 2, GL_FLOAT, GL_FALSE, 0, 0);
  delete [] velField;
  
  // Select shader program and load uniforms.
  glUseProgram(_velocityProgram);
  glUniformMatrix4fv(mvpIdx, 1, false, &_mvpMatrix[0][0]);

  // Draw the bound VAO.
  glDrawArrays(GL_POINTS, 0, cellCount);

  // Unbind VAO and Program.  Clean up memory.
  glBindVertexArray(0);
  glUseProgram(0);
}


void FancyRenderer::drawParticles(const std::vector<Vector2> &particles)
{
  // Get generic attribute and uniform indices.
  GLuint posIdx = glGetAttribLocation(_particleProgram, "position");
  GLuint colIdx = glGetUniformLocation(_particleProgram, "color");
  GLuint mvpIdx = glGetUniformLocation(_particleProgram, "mvpMatrix");

  // Bind the particles VAO.
  glBindVertexArray(_particlesVAO);

  // Load particle position values into a contiguous array.
  unsigned vertCount = particles.size();
  GLfloat *position = new GLfloat[vertCount * 2];
  vector<Vector2>::const_iterator itr = particles.begin();
  unsigned i = 0;
  for ( ; itr != particles.end(); ++itr) {
    position[i++] = itr->x;
    position[i++] = itr->y;
  }

  // Upload new data to the buffer object. Clean up memory.
  glEnableVertexAttribArray(posIdx);
  glBindBuffer(GL_ARRAY_BUFFER, _particlesVBO);
  glBufferData(GL_ARRAY_BUFFER, vertCount * 2 * sizeof(GLfloat), position, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(posIdx, 2, GL_FLOAT, GL_FALSE, 0, 0);
  delete [] position;

  // Select shader program and load uniforms.
  float vecColor[4] = {0.0f, 0.6f, 0.8f, 1.0f};
  glUseProgram(_particleProgram);
  glUniform4fv(colIdx, 1, vecColor);
  glUniformMatrix4fv(mvpIdx, 1, false, &_mvpMatrix[0][0]);

  // Draw the bound VAO.
  glDrawArrays(GL_POINTS, 0, vertCount);

  // Unbind particles VAO and Program. Clean up memory.
  glBindVertexArray(0);
  glUseProgram(0);
}
