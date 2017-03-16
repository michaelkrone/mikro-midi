#pragma once

#include "IoDefs.h"

BEGIN_IO_NAMESPACE

class Pin {
  protected:
  ioPin mPin;
  bool mPullup;
  bool mAnalog;

  public:
  Pin(ioPin pin, bool pullup = false, bool analog = false)
    : mPin(pin)
    , mPullup(pullup)
    , mAnalog(analog) {}

  virtual ~Pin() {}
};

END_IO_NAMESPACE
