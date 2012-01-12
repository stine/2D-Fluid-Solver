QT      += core gui opengl
CONFIG  += warn_on

OBJECTS_DIR = tmp
MOC_DIR     = tmp
UI_DIR      = tmp

INCLUDEPATH += ../ui \
               ../solver \
               ../renderers

SOURCES += ../ui/MainWindow.cpp \
           ../ui/QRendererWidget.cpp \
           ../solver/FluidSolver.cpp \
           ../solver/Grid.cpp \
           ../solver/Cell.cpp \
           ../renderers/CompatibilityRenderer.cpp

HEADERS += ../ui/MainWindow.h \
           ../ui/QRendererWidget.h \
           ../solver/Vector.h \
           ../solver/Cell.h \
           ../solver/FluidSolver.h \
           ../solver/Grid.h \
           ../renderers/IFluidRenderer.h \
           ../renderers/CompatibilityRenderer.h
