CONFIG  += ordererd

TEMPLATE = subdirs
SUBDIRS  = solver \
           ui \
           renderers \
           tests \
           main

tests.depends = solver ui renderers
main.depends  = solver ui renderers
