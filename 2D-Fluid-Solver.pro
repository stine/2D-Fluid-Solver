#-------------------------------------------------
#
# Project created by QtCreator 2011-12-27T14:52:43
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = 2D-Fluid-Solver
TEMPLATE = app


SOURCES += main.cpp \
           MainWindow.cpp \
           Cell.cpp \
           FluidSolver.cpp \
           Grid.cpp \
           FluidRenderer.cpp \
           CompatibilityRenderer.cpp

HEADERS  += MainWindow.h \
            Vector.h \
            Cell.h \
            FluidSolver.h \
            Grid.h \
            FluidRenderer.h \
            CompatibilityRenderer.h
