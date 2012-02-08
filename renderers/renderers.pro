include(../config/include.pri)

TEMPLATE = lib
TARGET   = renderers

HEADERS += IFluidRenderer.h \
           CompatibilityRenderer.h

SOURCES += CompatibilityRenderer.cpp
