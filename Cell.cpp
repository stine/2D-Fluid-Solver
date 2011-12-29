#include "Cell.h"
#include <cstddef>

Cell2D::Cell2D()
  : _pressure(0.0f),
    _isLiquid(false),
    _hasAllNeighbors(false)
{
  // Initialize array data members.
  for (unsigned i = 0; i < C2D_DIM; ++i) {
    _velocity[i] = 0.0f;
    _stagedVelocity[i] = 0.0f;
  }
  for (unsigned i = 0; i < C2D_NEI; ++i) {
    _neighbor[i] = NULL;
  }
}


void Cell2D::setLinkage(Cell2D *negXNeighbor,
			Cell2D *posXNeighbor,
			Cell2D *negYNeighbor,
			Cell2D *posYNeighbor)
{
  // Set all neighbor pointers.
  _neighbor[0] = negXNeighbor;
  _neighbor[1] = posXNeighbor;
  _neighbor[2] = negYNeighbor;
  _neighbor[3] = posYNeighbor;  

  // Set neighbor flag.
  _hasAllNeighbors = true;
  for (unsigned i = 0; i < C2D_NEI; ++i) {
    if (_neighbor[i] == NULL) {
      _hasAllNeighbors = false;
      break;
    }
  }
}


void Cell2D::unsetLinkage()
{
  // Unset all neighbor pointers.
  _neighbor[0] = NULL;
  _neighbor[1] = NULL;
  _neighbor[2] = NULL;
  _neighbor[3] = NULL;

  // Set flag to 'false'.
  _hasAllNeighbors = false;
}


bool Cell2D::getIsLiquid() const
{
  return _isLiquid;
}


void Cell2D::setIsLiquid(bool isLiquid)
{
  _isLiquid = isLiquid;
}