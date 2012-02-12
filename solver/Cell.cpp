#include "Cell.h"

Cell::Cell()
  : pressure(0.0f),
    cellType(AIR),
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


void Cell::commitStagedVel()
{
  memcpy(vel, stagedVel, sizeof(float) * DIM_COUNT);
}
