include(../config/include.pri)

TEMPLATE = app
TARGET   = 2D-Fluid-Solver

SOURCES += main.cpp

DEPENDENCY_LIBRARIES = solver \
                       ui \
                       renderers
include(../config/dependencies.pri)
