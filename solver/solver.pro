include(../config/include.pri)

TEMPLATE = lib
TARGET   = solver

HEADERS += Vector.h \
           FluidSolver.h \
           Grid.h \
           Cell.h

SOURCES += FluidSolver.cpp \
           Grid.cpp \
           Cell.cpp
