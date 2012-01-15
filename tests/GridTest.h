#ifndef __GRID_TEST__
#define __GRID_TEST__

#include <gtest/gtest.h>
#include <cmath>
#include "Grid.h"

#define TEST_GRID_WIDTH  3
#define TEST_GRID_HEIGHT 3

// Test fixture for the Grid test.
class GridTest : public testing::Test {
protected:
  const Grid defaultGrid;
  Grid testGrid;

  // Default constructor for the GridTest test fixture.
  GridTest()
    : defaultGrid(TEST_GRID_HEIGHT, TEST_GRID_WIDTH),
      testGrid(TEST_GRID_HEIGHT, TEST_GRID_WIDTH)
  {
    // Initialize a velocity field for testing.
    // The test grid is 3x3, and all cells contain liquid and 1.0f pressure.
    // The grid's origin is in the bottom left corner, (x,y) == (0,0).
    // Initial velocity is given to grid cells such that the X and Y 
    // velocity components are equal to the cell's position within the grid.
    // For example, cell (0,0) has an X velocity of 0, and Y velocity of 0.
    // Cell (1, 2) has an X velocity of 1, and Y velocity of 2.
    for (unsigned y = 0; y < TEST_GRID_HEIGHT; ++y) {
      for (unsigned x = 0; x < TEST_GRID_WIDTH; ++x) {
	testGrid(x, y).isLiquid = true;
	testGrid(x, y).pressure = 1.0f;
	testGrid(x, y).vel[Cell::X] = static_cast<float>(x);
	testGrid(x, y).vel[Cell::Y] = static_cast<float>(y);
      }
    }
  }

  // Compares data values in two cells; doesn't consider neighbor pointers.
  bool sameCellData(const Cell &a, const Cell &b)
  {
    bool result = true;
    result = result &&
      (a.pressure == b.pressure) &&
      (a.isLiquid == b.isLiquid) &&
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
  EXPECT_EQ(TEST_GRID_HEIGHT, defaultGrid.getRowCount());
  EXPECT_EQ(TEST_GRID_WIDTH, defaultGrid.getColCount());
}

TEST_F(GridTest, GetRowColCount)
{
  EXPECT_EQ(TEST_GRID_HEIGHT, defaultGrid.getRowCount());
  EXPECT_EQ(TEST_GRID_WIDTH, defaultGrid.getColCount());
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
  const unsigned i = TEST_GRID_WIDTH * y + x;
  EXPECT_EQ(&testGrid[i], &testGrid(x, y));
}

TEST_F(GridTest, SetCellLinkage)
{
  // All cells other than the far right column and top row will have a 
  // complete set of neighbors.  Verify that all linkage was created correctly.
  for (unsigned y = 0; y < TEST_GRID_HEIGHT - 1; ++y) {
    for (unsigned x = 0; x < TEST_GRID_WIDTH - 1; ++x) {
      EXPECT_TRUE(testGrid(x, y).allNeighbors);
      EXPECT_EQ(&testGrid(x+1, y),   testGrid(x, y).neighbors[Cell::POS_X]);
      EXPECT_EQ(&testGrid(x,   y+1), testGrid(x, y).neighbors[Cell::POS_Y]);
      EXPECT_EQ(&testGrid(x+1, y+1), testGrid(x, y).neighbors[Cell::POS_XY]);
    }
  }

  // Test the far right column.
  for (unsigned y = 0; y < TEST_GRID_HEIGHT; ++y) {
    unsigned x = TEST_GRID_WIDTH - 1;
    EXPECT_FALSE(testGrid(x, y).allNeighbors);
    EXPECT_EQ(NULL, testGrid(x, y).neighbors[Cell::POS_X]);
    EXPECT_EQ(NULL, testGrid(x, y).neighbors[Cell::POS_Y]);
    EXPECT_EQ(NULL, testGrid(x, y).neighbors[Cell::POS_XY]);
  }

  // Test the far top row..
  for (unsigned x = 0; x < TEST_GRID_WIDTH; ++x) {
    unsigned y = TEST_GRID_HEIGHT - 1;
    EXPECT_FALSE(testGrid(x, y).allNeighbors);
    EXPECT_EQ(NULL, testGrid(x, y).neighbors[Cell::POS_X]);
    EXPECT_EQ(NULL, testGrid(x, y).neighbors[Cell::POS_Y]);
    EXPECT_EQ(NULL, testGrid(x, y).neighbors[Cell::POS_XY]);
  }
}

TEST_F(GridTest, GetVelocity)
{
  // TODO, pass test by default.
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
