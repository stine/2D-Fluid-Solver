#ifndef __GRID_TEST__
#define __GRID_TEST__

#include <gtest/gtest.h>
#include <cmath>
#include "Grid.h"

#define TEST_GRID_WIDTH  3.0f
#define TEST_GRID_HEIGHT 3.0f

// Test fixture for the Grid test.
class GridTest : public testing::Test {
protected:
  const Grid defaultGrid;
  Grid testGrid;
  Grid edgeTestGrid;

  // Default constructor for the GridTest test fixture.
  GridTest()
    : defaultGrid(TEST_GRID_HEIGHT, TEST_GRID_WIDTH),
      testGrid(TEST_GRID_HEIGHT, TEST_GRID_WIDTH),
      edgeTestGrid(TEST_GRID_HEIGHT, TEST_GRID_WIDTH)
  {
    // Initialize a velocity field for testing.
    // The simulation area is [0.0f, 3.0] in each dimension, resulting in
    // a grid that internally stores 4 rows and 4 cols of MAC cells.
    // All cells contain liquid and 1.0f pressure.
    // The grid's origin is in the bottom left corner, (x,y) == (0,0).
    // Initial velocity is given to grid cells such that the X and Y 
    // velocity components are equal to the cell's position within the grid.
    // For example, cell (0,0) has an X velocity of 0, and Y velocity of 0.
    // Cell (1, 2) has an X velocity of 1, and Y velocity of 2.
    for (unsigned y = 0; y < testGrid.getRowCount(); ++y) {
      for (unsigned x = 0; x < testGrid.getColCount(); ++x) {
	testGrid(x, y).cellType = Cell::FLUID;
	testGrid(x, y).pressure = 1.0f;
	testGrid(x, y).vel[Cell::X] = static_cast<float>(x);
	testGrid(x, y).vel[Cell::Y] = static_cast<float>(y);
      }
    }

    // Copy the above testGrid into edgeTestGrid, but modify some values.
    // Specifically, set the y-velocities in the far right column to 0,
    // and set the x-velocities in the top row to 0.
    edgeTestGrid = testGrid;
    unsigned rightCol = edgeTestGrid.getColCount() - 1;
    unsigned topRow   = edgeTestGrid.getRowCount() - 1;
    for (unsigned y = 0; y < edgeTestGrid.getRowCount(); ++y) {
      edgeTestGrid(rightCol, y).vel[Cell::Y] = 0.0f;
    }
    for (unsigned x = 0; x < edgeTestGrid.getColCount(); ++x) {
      edgeTestGrid(x, topRow).vel[Cell::X] = 0.0f;
    }
  }

  // Compares data values in two cells; doesn't consider neighbor pointers.
  bool sameCellData(const Cell &a, const Cell &b)
  {
    bool result = true;
    result = result &&
      (a.pressure == b.pressure) &&
      (a.cellType == b.cellType) &&
      (a.allNeighbors == b.allNeighbors);
    for (int i = 0; i < Cell::DIM_COUNT; ++i) {
      result = result && 
	(a.vel[i] == b.vel[i]) &&
	(a.stagedVel[i] == b.stagedVel[i]);
    }
    return result;
  }
};



TEST_F(GridTest, DefaultConstructor)
{
  unsigned height = TEST_GRID_HEIGHT;
  unsigned width = TEST_GRID_WIDTH;
  unsigned rows = height < 3 ? 3 : ceil(height) + 1;
  unsigned cols = width  < 3 ? 3 : ceil(width)  + 1;
  EXPECT_EQ(rows, defaultGrid.getRowCount());
  EXPECT_EQ(cols, defaultGrid.getColCount());
}

TEST_F(GridTest, GetRowColCount)
{
  unsigned height = TEST_GRID_HEIGHT;
  unsigned width = TEST_GRID_WIDTH;
  unsigned rows = height < 3 ? 3 : ceil(height) + 1;
  unsigned cols = width  < 3 ? 3 : ceil(width)  + 1;
  EXPECT_EQ(rows, defaultGrid.getRowCount());
  EXPECT_EQ(cols, defaultGrid.getColCount());
}

TEST_F(GridTest, CopyConstructor)
{
  Grid copyGrid(testGrid);
  ASSERT_EQ(testGrid.getRowCount(), copyGrid.getRowCount());
  ASSERT_EQ(testGrid.getColCount(), copyGrid.getColCount());
  const unsigned size = testGrid.getRowCount() & testGrid.getColCount();
  for (unsigned i = 0; i < size; ++i) {
    EXPECT_TRUE(sameCellData(testGrid[i], copyGrid[i]));
  }
}

TEST_F(GridTest, Assignment)
{
  Grid assignGrid = testGrid;
  ASSERT_EQ(testGrid.getRowCount(), assignGrid.getRowCount());
  ASSERT_EQ(testGrid.getColCount(), assignGrid.getColCount());
  const unsigned size = testGrid.getRowCount() & testGrid.getColCount();
  for (unsigned i = 0; i < size; ++i) {
    EXPECT_TRUE(sameCellData(testGrid[i], assignGrid[i]));
  }
}

TEST_F(GridTest, Accessors)
{
  const unsigned x = 2;
  const unsigned y = 1;
  const unsigned i = testGrid.getColCount() * y + x;
  EXPECT_EQ(&testGrid[i], &testGrid(x, y));
}

TEST_F(GridTest, SetCellLinkage)
{
  unsigned x, y;

  // All cells other than the far right column and top row will have a 
  // complete set of neighbors.  Verify that all linkage was created correctly.
  for (y = 0; y < testGrid.getRowCount() - 1; ++y) {
    for (x = 0; x < testGrid.getColCount() - 1; ++x) {
      EXPECT_TRUE(testGrid(x, y).allNeighbors);
      EXPECT_EQ(&testGrid(x+1, y),   testGrid(x, y).neighbors[Cell::POS_X]);
      EXPECT_EQ(&testGrid(x,   y+1), testGrid(x, y).neighbors[Cell::POS_Y]);
      EXPECT_EQ(&testGrid(x+1, y+1), testGrid(x, y).neighbors[Cell::POS_XY]);
    }
  }

  // Test the far right column, except for the top right corner.
  for (y = 0; y < testGrid.getRowCount() - 1; ++y) {
    x = testGrid.getColCount() - 1;
    EXPECT_FALSE(testGrid(x, y).allNeighbors);
    EXPECT_EQ(NULL,              testGrid(x, y).neighbors[Cell::POS_X]);
    EXPECT_EQ(&testGrid(x, y+1), testGrid(x, y).neighbors[Cell::POS_Y]);
    EXPECT_EQ(NULL,              testGrid(x, y).neighbors[Cell::POS_XY]);
  }

  // Test the far top row, except for the top right corner.
  for (x = 0; x < testGrid.getColCount() - 1; ++x) {
    y = testGrid.getRowCount() - 1;
    EXPECT_FALSE(testGrid(x, y).allNeighbors);
    EXPECT_EQ(&testGrid(x+1, y), testGrid(x, y).neighbors[Cell::POS_X]);
    EXPECT_EQ(NULL,              testGrid(x, y).neighbors[Cell::POS_Y]);
    EXPECT_EQ(NULL,              testGrid(x, y).neighbors[Cell::POS_XY]);
  }

  // Test the top right corner.
  y = testGrid.getColCount() - 1;
  y = testGrid.getRowCount() - 1;
  EXPECT_FALSE(testGrid(x, y).allNeighbors);
  EXPECT_EQ(NULL, testGrid(x, y).neighbors[Cell::POS_X]);
  EXPECT_EQ(NULL, testGrid(x, y).neighbors[Cell::POS_Y]);
  EXPECT_EQ(NULL, testGrid(x, y).neighbors[Cell::POS_XY]);
}

TEST_F(GridTest, GetVelocity)
{
  // Test velocity at (0.0, 0.0)
  EXPECT_EQ(0.0f, testGrid.getVelocity(0.0, 0)(0));
  EXPECT_EQ(0.0f, testGrid.getVelocity(0.0, 0)(1));

  // Test velocity at (1.5, 1.0)
  EXPECT_EQ(1.5f, testGrid.getVelocity(1.5, 1.0)(0));
  EXPECT_EQ(1.0f, testGrid.getVelocity(1.5, 1.0)(1));

  // Test velocity at (1.0, 1.5)
  EXPECT_EQ(1.0f, testGrid.getVelocity(1.0, 1.5)(0));
  EXPECT_EQ(1.5f, testGrid.getVelocity(1.0, 1.5)(1));

  // Test velocity at (2.5, 1.5)
  EXPECT_EQ(2.5f, testGrid.getVelocity(2.5, 1.5)(0));
  EXPECT_EQ(1.5f, testGrid.getVelocity(2.5, 1.5)(1));

  // Test velocity at (3.0, 0.0)
  EXPECT_EQ(3.0f, testGrid.getVelocity(3.0, 0.0)(0));
  EXPECT_EQ(0.0f, testGrid.getVelocity(3.0, 0.0)(1));

  // Test velocity at (0.0, 3.0)
  EXPECT_EQ(0.0f, testGrid.getVelocity(0.0, 3.0)(0));
  EXPECT_EQ(3.0f, testGrid.getVelocity(0.0, 3.0)(1));

  // Test velocity at (2.5, 3.0)
  EXPECT_EQ(2.5f, testGrid.getVelocity(2.5, 3.0)(0));
  EXPECT_EQ(3.0f, testGrid.getVelocity(2.5, 3.0)(1));

  // Test velocity at (3.0, 3.0)
  EXPECT_EQ(3.0f, testGrid.getVelocity(3.0, 3.0)(0));
  EXPECT_EQ(3.0f, testGrid.getVelocity(3.0, 3.0)(1));

  // Test velocity at (-5.0, -5.0)
  EXPECT_EQ(0.0f, testGrid.getVelocity(-5.0, -5.0)(0));
  EXPECT_EQ(0.0f, testGrid.getVelocity(-5.0, -5.0)(1));

  // Test velocity at (100.0, 100.0)
  EXPECT_EQ(3.0f, testGrid.getVelocity(100.0, 100.0)(0));
  EXPECT_EQ(3.0f, testGrid.getVelocity(100.0, 100.0)(1));

  // Use edgeTestGrid to test velocity at (3.0, 1.25)
  EXPECT_EQ(3.0f,   edgeTestGrid.getVelocity(3.0, 1.25)(0));
  EXPECT_EQ(0.625f, edgeTestGrid.getVelocity(3.0, 1.25)(1));

  // Use edgeTestGrid to test velocity at (3.0, 2.0)
  EXPECT_EQ(3.0f, edgeTestGrid.getVelocity(3.0, 2.0)(0));
  EXPECT_EQ(1.0f, edgeTestGrid.getVelocity(3.0, 2.0)(1));

  // Use edgeTestGrid to test velocity at (3.0, 3.0)
  EXPECT_EQ(1.5f, edgeTestGrid.getVelocity(3.0, 3.0)(0));
  EXPECT_EQ(1.5f, edgeTestGrid.getVelocity(3.0, 3.0)(1));
}

TEST_F(GridTest, GetPressureGradient)
{
  // TODO, pass test by default.
}

TEST_F(GridTest, GetVelocityDivergence)
{
  // TODO, pass test by default.
}

#endif // __GRID_TEST__
