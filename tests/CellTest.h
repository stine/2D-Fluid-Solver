#ifndef __CELL_TEST__
#define __CELL_TEST__

#include <gtest/gtest.h>
#include <cstddef>
#include "Cell.h"


// Test fixture for the Cell test.
class CellTest : public testing::Test {
protected:
  const Cell defaultCell;
  Cell testCell;

  virtual void SetUp() {
    testCell.pressure = 5.0f;
    testCell.vel[Cell::X] = 1.0f;
    testCell.vel[Cell::Y] = 2.0f;
    testCell.stagedVel[Cell::X] = 3.0f;
    testCell.stagedVel[Cell::Y] = 5.0f;
    testCell.isLiquid = true;
    testCell.allNeighbors = true;
    return;
  }

  virtual void TearDown() {
    // Nothing to do for teardown.
    return;
  }
};

TEST_F(CellTest, DefaultConstructor)
{
  EXPECT_EQ(0.0f, defaultCell.pressure);
  for (int i = 0; i < Cell::DIM_COUNT; ++i) {
    EXPECT_EQ(0.0f, defaultCell.vel[i]);
    EXPECT_EQ(0.0f, defaultCell.stagedVel[i]);
  }
  EXPECT_FALSE(defaultCell.isLiquid);
  EXPECT_FALSE(defaultCell.allNeighbors);
  for (int i = 0; i < Cell::NEIGHBOR_COUNT; ++i) {
    EXPECT_EQ(NULL, defaultCell.neighbors[i]);
  }
}

TEST_F(CellTest, CopyConstructor)
{
  Cell newCell(testCell);

  EXPECT_EQ(testCell.pressure, newCell.pressure);
  for (int i = 0; i < Cell::DIM_COUNT; ++i) {
    EXPECT_EQ(testCell.vel[i], newCell.vel[i]);
    EXPECT_EQ(testCell.stagedVel[i], newCell.stagedVel[i]);
  }
  EXPECT_EQ(testCell.isLiquid, newCell.isLiquid);
  EXPECT_FALSE(newCell.allNeighbors);
  for (int i = 0; i < Cell::NEIGHBOR_COUNT; ++i) {
    EXPECT_EQ(NULL, newCell.neighbors[i]);
  }
}

TEST_F(CellTest, Assignment)
{
  Cell newCell = testCell;

  EXPECT_EQ(testCell.pressure, newCell.pressure);
  for (int i = 0; i < Cell::DIM_COUNT; ++i) {
    EXPECT_EQ(testCell.vel[i], newCell.vel[i]);
    EXPECT_EQ(testCell.stagedVel[i], newCell.stagedVel[i]);
  }
  EXPECT_EQ(testCell.isLiquid, newCell.isLiquid);
  EXPECT_FALSE(newCell.allNeighbors);
  for (int i = 0; i < Cell::NEIGHBOR_COUNT; ++i) {
    EXPECT_EQ(NULL, newCell.neighbors[i]);
  }
}

TEST_F(CellTest, CommitStagedVel)
{
  testCell.commitStagedVel();
  
  for (int i = 0; i < Cell::DIM_COUNT; ++i) {
    EXPECT_EQ(testCell.stagedVel[i], testCell.vel[i]);
  }
}

#endif // __CELL_TEST__