include(../sources.pri)

TEMPLATE = app
TARGET   = solver-tests
CONFIG(release, debug|release) {
    QMAKE_POST_LINK=./$$DESTDIR/$$TARGET
}

HEADERS += Vector2Test.h \
	   CellTest.h \
	   GridTest.h

SOURCES += tests.cpp

LIBS    += -lgtest
