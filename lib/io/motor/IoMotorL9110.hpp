#pragma once

#include "IoDefs.h"
#include "ic/IoIcL9110.hpp"
#include "motor/IoMotorDriver.hpp"

BEGIN_IO_NAMESPACE

class MotorL9110 : public MotorDriver {
  protected:
    IcL9110& mIcL9110;
    uint8_t mIndex;

  public:
    MotorL9110(IcL9110& icL9110, uint8_t index = 0)
      : mIcL9110(icL9110)
      , mIndex(index) {}

    virtual ~MotorL9110() {}

    inline void enable() {}

    inline void disable() {}

    inline void up(int speed = ANALOG_MAX) {
      mIcL9110.up(mIndex, speed);
    }

    inline void down(int speed = ANALOG_MAX) {
      mIcL9110.down(mIndex, speed);
    }

    inline void stop() {
      mIcL9110.stop(mIndex);
    }

    inline void halt() {
      mIcL9110.halt(mIndex);
    }
  };

END_IO_NAMESPACE
