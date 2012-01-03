#include "CompatibilityRenderer.h"
#include <cstdio>

CompatibilityRenderer::CompatibilityRenderer(const FluidSolver2D &solver,
					     QWidget *parent)
  : FluidRenderer(CompatibilityRenderer::getFormat(), parent),
    _solver(solver)
{}


CompatibilityRenderer::~CompatibilityRenderer()
{}


QGLFormat CompatibilityRenderer::getFormat()
{
  // Specify the necessary OpenGL context attributes for this renderer.
  QGLFormat glFormat;
  glFormat.setVersion(2, 1);
  return glFormat;
}


void CompatibilityRenderer::initializeGL()
{
  // Old-fashioned OpenGL here. Rewrite for CORE profile.
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glShadeModel(GL_SMOOTH);

  // Set the clear color.
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  // Setup scene orientation.
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0f, 0.0f, -10.0f);
}


void CompatibilityRenderer::resizeGL(int width, int height)
{
  // Calculate an appropriate orthographic projection matrix to display the sim.
  //----------------------------------------------------------------------------

  // Define a viewport based on widget size.
  glViewport(0, 0, width, height);

  // For the purpose of fitting the grid within the rendering area, take into
  // account a margin of 1 cell around the grid.
  unsigned gridWidth = _solver.getSimulationWidth() + 2;
  unsigned gridHeight = _solver.getSimulationHeight() + 2;

  // Set default values for the projection matrix.  Directly using these values
  // will result in the grid perfectly fitting within the rendering area, but
  // the grid will be stretched according to the aspect ratio.
  float xMin = -1.0f;
  float xMax = xMin + gridWidth;
  float yMin = -1.0;
  float yMax = yMin + gridHeight;
  
  // Calculate the ratio of rendering area pixels per cell along each axis.
  float pixPerCellW = float(width) / float(gridWidth);
  float pixPerCellH = float(height) / float(gridHeight);
  if (pixPerCellW < pixPerCellH) {
    // Width is the dominant dimension.
    float simHeight = float(gridWidth) * float(height) / float(width);
    yMin = -(simHeight - _solver.getSimulationHeight()) / 2;
    yMax = yMin + simHeight;
  }
  else {
    // Height is the dominant dimension.
    float simWidth = float(gridHeight) * float(width) / float(height);
    xMin = -(simWidth - _solver.getSimulationWidth()) / 2;
    xMax = xMin + simWidth;
  }
  
  // Define projection matrix based on widget size.
  glPushAttrib(GL_MATRIX_MODE);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(xMin, xMax, yMin, yMax, 5.0, 15.0);
  glPopAttrib();
}


void CompatibilityRenderer::paintGL()
{
  // Clear the existing framebuffer contents.
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Push the current modelview matrix onto the stack.
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  // Provide a self-reference to the fluid solver for rendering its data.
  _solver.draw(this);

  // Restore the previous modelview matrix.
  glPopMatrix();
}


void CompatibilityRenderer::drawGrid2D(const Grid2D &grid)
{
  float height = grid.getRowCount();
  float width  = grid.getColCount();

  // Store previous OpenGL state.
  glPushAttrib(GL_CURRENT_BIT);

  // Draw the vertical and horizontal grid lines.
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  glBegin(GL_LINES);
  for (unsigned i = 0; i <= grid.getColCount(); ++i) {
    glVertex2f(i, 0);
    glVertex2f(i, height);
  }
  for (unsigned i = 0; i <= grid.getRowCount(); ++i) {
    glVertex2f(0, i);
    glVertex2f(width, i);
  }
  glEnd();

  // Color the cells blue if they currently contain liquid.
  glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
  for (unsigned row = 0; row < grid.getRowCount(); ++row) {
    for (unsigned col = 0; col < grid.getColCount(); ++col) {
      if (grid(row, col).getIsLiquid()) {
	glBegin(GL_TRIANGLES);
	glVertex2f(col, row);
	glVertex2f(col+1, row);
	glVertex2f(col+1, row+1);
	glVertex2f(col+1, row+1);
	glVertex2f(col, row+1);
	glVertex2f(col, row);
	glEnd();
      }
    }
  }

  // Restore previous OpenGL state.
  glPopAttrib();

}
