#ifndef __Q_RENDERER_WIDGET_H__
#define __Q_RENDERER_WIDGET_H__

#include "GL3/gl3.h"
#include <QGLWidget>
#include "IFluidRenderer.h"

class QRendererWidget : public QGLWidget
{
  Q_OBJECT

public:
  IFluidRenderer *_renderer;

  // Enumerated type listing all implemented renderers to choose from.
  enum Renderers {
    COMPATIBILITY_RENDERER = 0,
    FANCY_RENDERER,
    RENDERER_COUNT
  };
  
  // Static factory method that, when provided with the desired parent
  // widget and a type of renderer, creates an OpenGL context and
  // attaches an instantiated FluidRenderer to it.  Returns a pointer
  // to the newly created QRendererWidget.
  //
  // The caller accepts ownership of the returned pointer.
  // 
  // Arguments:
  //   QWidget *parent - The parent widget.
  //   Renderers renderer - The renderer that should be used.
  static QRendererWidget * rendererWidget(QWidget *parent,
					  Renderers renderer);

  // Destructor
  //
  // Arguments:
  //   None
  virtual ~QRendererWidget();

  // Returns the recommended minimum size for this widget.
  // 
  // Inherited from QGLWidget.
  //
  // Arguments:
  //   None
  //
  // Returns:
  //   QSize - The recommended minimum size for this widget.
  virtual QSize minimumSizeHint() const;

  // Returns the recommended size for this widget.
  //
  // Inherited from QGLWidget.
  // 
  // Arguments:
  //   None
  //
  // Returns:
  //   QSize - The recommended size for this widget.
  virtual QSize sizeHint() const;

protected:
  // Induces renderer to call all initial OpenGL commands, including modelview
  // and projection matrix initial setup, geometry loading, etc.  Called once
  // before the first call to resizeGL or paintGL.
  //
  // Inherited from QGLWidget.
  //
  // Arguments:
  //   None
  //
  // Returns:
  //   None
  virtual void initializeGL();

  // Induces renderer to perform necessary logic to resize the widget.  Typically
  // involves redefining the viewport and projection matrix.  Called whenever 
  // the widget changes size.
  //
  // Inherited from QGLWidget.
  //
  // Arguments:
  //   int width - The new width of the widget, in pixels.
  //   int height - The new height of the widget, in pixels.
  //
  // Returns:
  //   None
  virtual void resizeGL(int width, int height);

  // Induces renderer to perform all rendering for the widget.  Called whenever
  // the widget needs to be redrawn. Typically initiated by a call to updateGL.
  //
  // Inherited from QGLWidget.
  //
  // Arguments:
  //   None
  //
  // Returns:
  //   None
  virtual void paintGL();

private:
  // This serves as a Qt widget wrapper around a FluidRenderer instance.
  // 
  // Arguments:
  //   QGLFormat &format - The requested format for the OpenGL context.
  //   QWidget *parent - The parent widget of this widget.
  //   IFluidRenderer *renderer - The instantiated FluidRenderer to use.
  QRendererWidget(const QGLFormat &format,
		  QWidget *parent,
		  IFluidRenderer *renderer);

};

#endif // __Q_RENDERER_WIDGET_H__
