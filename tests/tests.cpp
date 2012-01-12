#include <limits.h>
#include <gtest/gtest.h>

#include <vector>
#include "Grid.h"
#include "FluidSolver.h"

using namespace std;

// TODO - YUCK - This global variable is a temporary hack!!!
FluidSolver *solver = NULL;

GTEST_API_ int main(int argc, char *argv[])
{
  // Initialize a velocity field for testing.
  // Note: values in this velocity field are arbitrarily chosen and may be
  //  divergent within a cell.
  // Note: sin() is used to clamp output values to [-1, 1].
  const unsigned rows = 3;
  const unsigned cols = 3;
  const unsigned size = rows * cols;
  Grid grid(3, 3);
  for (unsigned i = 0; i < size; ++i) {
    grid[i].isLiquid = true;
    grid[i].pressure = 1.0f;
    grid[i].vel[Cell::X] = sin(i * 45.215 + 88.15468) / 2; // arbitrary constants
    grid[i].vel[Cell::Y] = sin(i * 2.548 + 121.1215) / 2;  // arbitrary constants
  }

  // Initialize GTest library.
  testing::InitGoogleTest(&argc, argv);

  // Run all unit tests!
  return RUN_ALL_TESTS();
}
