QT += opengl
CONFIG  += warn_on debug_and_release
CONFIG  -= app_bundle

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
