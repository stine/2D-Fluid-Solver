#include "Cell.h"

Cell::Cell()
  : pressure(0.0f),
    isLiquid(false),
    allNeighbors(false)
{
  // Initialize arrays.
  for (unsigned i = 0; i < DIM_COUNT; ++i) {
    vel[i] = 0.0f;
    stagedVel[i] = 0.0f;
  }
  for (unsigned i = 0; i < NEIGHBOR_COUNT; ++i) {
    neighbors[i] = NULL;
  }
}


Cell::Cell(const Cell &cell)
  : pressure(cell.pressure),
    isLiquid(cell.isLiquid),
    allNeighbors(false)
{
  // Copy arrays.
  memcpy(vel, cell.vel, DIM_COUNT * sizeof(float));
  memcpy(stagedVel, cell.stagedVel, DIM_COUNT * sizeof(float));

  // Initialize 'neighbor' pointers.
  for (unsigned i = 0; i < NEIGHBOR_COUNT; ++i) {
    neighbors[i] = NULL;
  }
}


Cell & Cell::operator=(const Cell &cell)
{
  if (this != &cell) {
    // Copy data members.
    pressure = cell.pressure;
    isLiquid = cell.isLiquid;
    memcpy(vel, cell.vel, DIM_COUNT * sizeof(float));
    memcpy(stagedVel, cell.stagedVel, DIM_COUNT * sizeof(float));
    
    // Initialize 'neighbor' pointers.
    allNeighbors = false;
    for (unsigned i = 0; i < NEIGHBOR_COUNT; ++i) {
      neighbors[i] = NULL;
    }
  }
  return *this;
}


void Cell::commitStagedVel()
{
  memcpy(vel, stagedVel, sizeof(float) * DIM_COUNT);
}
