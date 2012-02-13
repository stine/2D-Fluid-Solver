#include "QRendererWidget.h"
#include "CompatibilityRenderer.h"

// TODO - YUCK - This global variable is a temporary hack!!!
#include "FluidSolver.h"
extern FluidSolver *solver;

QRendererWidget * QRendererWidget::rendererWidget(QWidget *parent,
						  Renderers renderer)
{
  // Assert that sane arguments are provided.
  // TODO Assert(parent != NULL);
  // TODO Assert(renderer < RENDERER_COUNT);

  // Instantiate the appropriate renderer.
  IFluidRenderer *rendPtr = NULL;
  switch (renderer) {
  case COMPATIBILITY_RENDERER:
    rendPtr = new CompatibilityRenderer();
    break;
  default:
    // Shouldn't get here...
    break;
  }

  // Instantiate and return a QRendererWidget.
  return new QRendererWidget(rendPtr->getFormat(), parent, rendPtr);
}


QRendererWidget::QRendererWidget(const QGLFormat &format,
				 QWidget *parent,
				 IFluidRenderer *renderer)
  : QGLWidget(format, parent),
    _renderer(renderer)
{}


QRendererWidget::~QRendererWidget()
{
  delete _renderer;
  _renderer = NULL;
}


QSize QRendererWidget::minimumSizeHint() const
{
  return QSize(50, 50);
}


QSize QRendererWidget::sizeHint() const
{
  return QSize(400, 400);
}


void QRendererWidget::initializeGL()
{
  _renderer->initialize();
}


void QRendererWidget::resizeGL(int width, int height)
{
  _renderer->resize(width, height);
}


void QRendererWidget::paintGL()
{
  solver->draw(_renderer);
  update();
}
