! include(../sources.pri) {
  error(Couldn't find the sources.pri file)
}
! include(test-sources.pri) {
  error(Couldn't find the test-sources.pri file)
}

TEMPLATE = app
SOURCES += tests.cpp
LIBS    += -lgtest
TARGET   = solver-tests
QMAKE_POST_LINK=./$$TARGET

