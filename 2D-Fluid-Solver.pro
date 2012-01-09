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
           ./ui/QRendererWidget.cpp \
           ./solver/FluidSolver.cpp \
           ./solver/Grid.cpp \
           ./solver/Cell.cpp \
           ./renderers/CompatibilityRenderer.cpp

HEADERS  += ./ui/MainWindow.h \
            ./ui/QRendererWidget.h \
            ./solver/Vector.h \
            ./solver/Cell.h \
            ./solver/FluidSolver.h \
            ./solver/Grid.h \
            ./renderers/IFluidRenderer.h \
            ./renderers/CompatibilityRenderer.h
