#-------------------------------------------------
#
# Project created by QtCreator 2011-12-27T14:52:43
#
#-------------------------------------------------

TARGET    = 2D-Fluid-Solver

QT       += core gui opengl
CONFIG   += warn_on
TEMPLATE  = app

INCLUDEPATH += ./ui \
               ./solver \
               ./renderers

SOURCES += main.cpp \
           ./ui/MainWindow.cpp \
           ./solver/FluidSolver.cpp \
           ./solver/Grid.cpp \
           ./renderers/FluidRenderer.cpp \
           ./renderers/CompatibilityRenderer.cpp

HEADERS  += ./ui/MainWindow.h \
            ./solver/Vector.h \
            ./solver/Cell.h \
            ./solver/FluidSolver.h \
            ./solver/Grid.h \
            ./renderers/FluidRenderer.h \
            ./renderers/CompatibilityRenderer.h
