#include "CompatibilityRenderer.h"
#include <cstdio>

// TODO - YUCK - This global variable is a temporary hack!!!
#include "FluidSolver.h"
extern FluidSolver *solver;
using std::vector;

QGLFormat CompatibilityRenderer::getFormat()
{
  // Specify the necessary OpenGL context attributes for this renderer.
  QGLFormat glFormat;
  glFormat.setVersion(2, 1);
  glFormat.setSwapInterval(1);
  return glFormat;
}


void CompatibilityRenderer::initialize()
{
  // Old-fashioned OpenGL here. Rewrite for CORE profile.
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glShadeModel(GL_SMOOTH);
  glPointSize(2.0f);

  // Set the clear color.
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  // Setup scene orientation.
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0f, 0.0f, -10.0f);
}


void CompatibilityRenderer::resize(int pixWidth, int pixHeight)
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
  
  // Define projection matrix based on widget size.
  glPushAttrib(GL_MATRIX_MODE);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(xMin, xMax, yMin, yMax, 5.0, 15.0);
  glPopAttrib();
}


void CompatibilityRenderer::beginFrame()
{
  // Clear the existing framebuffer contents.
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Store previous OpenGL state.
  glPushAttrib(GL_CURRENT_BIT);
}


void CompatibilityRenderer::endFrame()
{
  // Pop OpenGL state.
  glPopAttrib();
}


void CompatibilityRenderer::drawGrid(const Grid &grid)
{
  // Get grid dimensions.
  float height = grid.getRowCount();
  float width  = grid.getColCount();

  // Set the color for grid lines.
  glColor4f(0.2f, 0.2f, 0.2f, 1.0f);

  // Draw the grid lines.
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


void CompatibilityRenderer::drawCells(const Grid &grid)
{
  // Get grid dimensions.
  float height = grid.getRowCount();
  float width  = grid.getColCount();
  
  // Set the MAC grid cell color.
  glColor4f(1.0f, 1.0f, 1.0f, 0.1f);

  // Draw the MAC grid cells if they contain fluid.
  glPushAttrib(GL_DEPTH_BUFFER_BIT);
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
  glPopAttrib();
}


void CompatibilityRenderer::drawVectors(const Grid &grid)
{
  // Get grid dimensions.
  float height = grid.getRowCount();
  float width  = grid.getColCount();
  
  // Set the MAC face velocity vector color.
  glColor4f(0.5f, 0.0f, 0.0f, 1.0f);

  // Draw the MAC face velocity vectors.
  for (unsigned y = 0; y < height; ++y) {
    for (unsigned x = 0; x < width; ++x) {
      float xV = grid(x, y).vel[Cell::X] * 0.5f;
      glBegin(GL_LINES);
      glVertex2f(x, y + 0.5f);
      glVertex2f(x + xV, y + 0.5f);
      glEnd();

      float yV = grid(x, y).vel[Cell::Y] * 0.5f;
      glBegin(GL_LINES);
      glVertex2f(x + 0.5f, y);
      glVertex2f(x + 0.5f, y + yV);
      glEnd();
    }
  }

  // Set the velocity field vector color.
  glColor4f(1.0f, 1.0f, 0.0f, 1.0f);

  // Draw the velocity vector at the center of each cell.
  for (float y = 0.5f; y < grid.getHeight(); y += 1.0f) {
    for (float x = 0.5f; x < grid.getWidth(); x += 1.0f) {
      Vector2 vec = grid.getVelocity(Vector2(x, y)) * 0.5;
      glBegin(GL_LINES);
      glVertex2f(x, y);
      glVertex2f(x + vec.x, y + vec.y);
      glEnd();
    }
  }
}


void CompatibilityRenderer::drawParticles(const std::vector<Vector2> &particles)
{
  // Set the color for the fluid particles.
  glColor4f(0.0f, 0.6f, 0.8f, 1.0f);
  
  // Draw the fluid particles.
  glBegin(GL_POINTS);
  vector<Vector2>::const_iterator itr = particles.begin();
  for(; itr != particles.end(); ++itr)
  {
    glVertex2f(itr->x, itr->y);
  }
  glEnd();
}
