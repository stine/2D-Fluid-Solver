include(../config/include.pri)

TEMPLATE = app
TARGET   = solver-tests

SOURCES += tests.cpp

HEADERS += Vector2Test.h \
	   CellTest.h \
	   GridTest.h

LIBS    += -lgtest

#CONFIG(release, debug|release) {
#    QMAKE_POST_LINK=./$$TARGET
#}

DEPENDENCY_LIBRARIES = solver
include(../config/dependencies.pri)
