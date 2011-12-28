#include "FluidRenderer.h"

FluidRenderer::FluidRenderer(const QGLFormat &format,
			     QWidget *parent)
    : QGLWidget(format, parent)
{}

FluidRenderer::~FluidRenderer()
{}

QSize FluidRenderer::minimumSizeHint() const
{
  return QSize(50, 50);
}

QSize FluidRenderer::sizeHint() const
{
  return QSize(400, 400);
}
