# Lifted from user Travis Grockel on StackOverflow.
# This function sets up the dependencies for libraries that are built with
# this project.  Specify the libraries you need to depend on in the variable
# DEPENDENCY_LIBRARIES and this will add them.

include(include.pri)

SONAME=so

for(dep, DEPENDENCY_LIBRARIES) {
#    message($$TARGET depends on $$dep ($${DESTDIR}/$${dep}.$${SONAME}))
    LIBS += $${DESTDIR}/lib$${dep}.$${SONAME}
    PRE_TARGETDEPS += $${DESTDIR}/lib$${dep}.$${SONAME}
}