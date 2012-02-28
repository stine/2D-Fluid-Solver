# 2D Semi-Lagrangian Fluid Solver

Here is an attempt at creating a Semi-Lagrangian fluid solver in two dimensions, implemented as suggested by [Robert Bridson](http://www.cs.ubc.ca/~rbridson/) in his book [Fluid Simulation for Computer Graphics](http://www.amazon.com/Fluid-Simulation-Robert-Bridson/dp/1568813260).

This started as an exploratory effort between [Jason Burton](http://github.com/mightcouldb1) and [Matt Stine](http://github.com/stine), and the intent is for us to learn exactly how fluid simulations are made through building one from scratch.  Most of the focus is on simply understanding the data structures and algorithms employed; this is not yet an exercise in optimization.


# Features

Implemented features include:
* Marker-and-Cell Grid data structures
* Bilinear interpolation of staggered MAC Grid velocities
* Velocity advection via backward particle trace
* Particle advection
* A "compatibility" renderer for visualizing data

Work to do:
* Good-looking renderer for OpenGL Core profiles
* Boundary condition enforcement
* Pressure solve (incompressibility requirement)
* Improved UI with a better layout and more user controls
* Encapsulate advection, pressure solve, and Cell references in the Grid class


# Compilation

## Dependencies

Our fluid solver uses Qt 4 and QMake to create GUIs and automatically generate Makefiles.  Both of these are provided by the standard Qt development package.  Linux users can typically find this in their system's package manager (e.g. `sudo yum install qt-devel` on Fedora).  Everyone else can download the Qt 4 SDK at [Qt's download page](http://qt.nokia.com/downloads/).

We also use the Google Test framework in this project.  Again, Linux users can typically find this in their package manager (e.g. `sudo yum install gtest-devel` on Fedora), and everyone else can download it from the [Google Test project page](http://code.google.com/p/googletest/).

## Compiling

Once all dependencies are resolved, change to the top-level directory for the 2D fluid solver project.  Running QMake in this directory will generate the necessary Makefiles.

   qmake-qt4

### Release

Building for release will produce an optitmized binary and will automatically execute all unit tests at compile time.  The build system will fail to produce an executable if any of the unit tests do not succeed.  A "release" directory will be created and populated with the fluid solver executable.

   make release 

### Debug

Building for debug will produce an unoptitmized binary with debugging symbols.  The unit tests will be compiled, but not executed, and the fluid solver executable will be produced upon successful compilation.  A "debug" directory will be created under the top-level directory.

   make debug

