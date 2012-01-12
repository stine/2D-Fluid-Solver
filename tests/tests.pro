! include(../sources.pri) {
  error(Couldn't find the sources.pri file)
}

TEMPLATE = app
SOURCES += tests.cpp
LIBS    += -lgtest
TARGET   = solver-tests

