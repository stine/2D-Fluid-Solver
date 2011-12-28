#include "FluidRenderer2D.h"

FluidRenderer2D::FluidRenderer2D(const FluidSolver2D *solver,
				 QWidget *parent)
  : FluidRenderer(FluidRenderer2D::getFormat(), parent),
    _solver(solver)
{}


FluidRenderer2D::~FluidRenderer2D()
{}


QGLFormat FluidRenderer2D::getFormat()
{
  // Specify the necessary OpenGL context attributes for this renderer.
  QGLFormat glFormat;
  glFormat.setVersion(2, 1);
  return glFormat;
}


void FluidRenderer2D::initializeGL()
{
  // TODO old-fashioned OpenGL here. Rewrite for CORE profile in the future.
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glShadeModel(GL_SMOOTH);

  // Setup scene orientation.
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -10.0);
}


void FluidRenderer2D::resizeGL(int width, int height)
{
  // Define viewport based on widget size.
  int side = qMin(width, height);
  glViewport((width - side) / 2, (height - side) / 2, side, side); // What?
  
  // Define projection matrix based on widget size.
  glPushAttrib(GL_MATRIX_MODE);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-5, +5, -5, +5, 5.0, 15.0); // TODO appropriate for grid
  glPopAttrib();
}


void FluidRenderer2D::paintGL()
{
  // Clear the existing framebuffer contents.
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Push the current modelview matrix onto the stack.
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  // Provide a self-reference to the fluid solver for rendering its data.
  _solver->draw(this);

  // Restore the previous modelview matrix.
  glPopMatrix();
}


void FluidRenderer2D::drawGrid2D(const Grid2D &grid)
{

}
