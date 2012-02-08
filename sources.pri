include(../config.pri)

BaseDirectory = ..

Release:DESTDIR     = $$BaseDirectory/release
Release:OBJECTS_DIR = $$BaseDirectory/release/.obj
Release:MOC_DIR     = $$BaseDirectory/release/.moc
Release:RCC_DIR     = $$BaseDirectory/release/.rcc
Release:UI_DIR      = $$BaseDirectory/release/.ui

Debug:DESTDIR     = $$BaseDirectory/debug
Debug:OBJECTS_DIR = $$BaseDirectory/debug/.obj
Debug:MOC_DIR     = $$BaseDirectory/debug/.moc
Debug:RCC_DIR     = $$BaseDirectory/debug/.rcc
Debug:UI_DIR      = $$BaseDirectory/debug/.ui

INCLUDEPATH += $$BaseDirectory/ui \
               $$BaseDirectory/solver \
               $$BaseDirectory/renderers

SOURCES += $$BaseDirectory/ui/MainWindow.cpp \
           $$BaseDirectory/ui/QRendererWidget.cpp \
           $$BaseDirectory/solver/FluidSolver.cpp \
           $$BaseDirectory/solver/Grid.cpp \
           $$BaseDirectory/solver/Cell.cpp \
           $$BaseDirectory/renderers/CompatibilityRenderer.cpp

HEADERS += $$BaseDirectory/ui/MainWindow.h \
           $$BaseDirectory/ui/QRendererWidget.h \
           $$BaseDirectory/solver/Vector.h \
           $$BaseDirectory/solver/Cell.h \
           $$BaseDirectory/solver/FluidSolver.h \
           $$BaseDirectory/solver/Grid.h \
           $$BaseDirectory/renderers/IFluidRenderer.h \
           $$BaseDirectory/renderers/CompatibilityRenderer.h
