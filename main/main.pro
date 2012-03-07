include(../sources.pri)

# Copy shaders to the application directory.
shaders.commands += @echo Copying shaders to application directory. Done. &
shaders.commands += $$quote(cp -rf $${ShaderDir} $${DESTDIR})
QMAKE_EXTRA_TARGETS += shaders
POST_TARGETDEPS += shaders

# Build the application.
TEMPLATE = app
SOURCES += main.cpp
TARGET   = 2D-Fluid-Solver



