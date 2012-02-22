#include "SignalRelay.h"

SignalRelay * SignalRelay::_instance = NULL;

SignalRelay * SignalRelay::getInstance()
{
  if (!_instance)
    _instance = new SignalRelay;
  return _instance;
}
