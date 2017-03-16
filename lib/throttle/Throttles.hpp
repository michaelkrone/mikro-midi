#pragma once

#include "ThrottleDefs.h"

BEGIN_THROTTLE_NAMESPACE

class Throttles {
private:
  ThrottleItem mItems[MAX_THROTTLES];

public:
  Throttles() {}
  virtual ~Throttles() {}

  bool set(uint8_t index, uint32_t ms) {
    if (index >= MAX_THROTTLES) {
      return false;
    }

    mItems[index] = {ms, millis()};
    return true;
  }

  inline bool shouldUpdate(uint8_t index) {
    if (index >= MAX_THROTTLES) {
      return false;
    }
    return (millis() - mItems[index].lastReset) >= mItems[index].threshold;
  }

  inline void reset(uint8_t index) {
    if (index >= MAX_THROTTLES) {
      return false;
    }
    mItems[index].lastReset = millis();
  }

};

END_THROTTLE_NAMESPACE
