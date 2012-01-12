! include(../sources.pri) {
  error(Couldn't find the sources.pri file)
}

TEMPLATE = app
SOURCES += main.cpp
TARGET   = ../2D-Fluid-Solver
