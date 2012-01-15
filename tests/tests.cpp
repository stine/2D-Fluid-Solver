#include <limits.h>
#include <gtest/gtest.h>

#include <vector>
#include "Grid.h"
#include "FluidSolver.h"

// Include test headers here:
#include "Vector2Test.h"
#include "CellTest.h"
#include "GridTest.h"

// TODO - YUCK - This global variable is a temporary hack!!!
FluidSolver *solver = NULL;

GTEST_API_ int main(int argc, char *argv[])
{
  // Initialize GTest library.
  testing::InitGoogleTest(&argc, argv);

  // Run all unit tests!
  return RUN_ALL_TESTS();
}
