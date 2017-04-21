#pragma once

#include "IoDefs.h"
#include "ic/IoIcL293.hpp"
#include "motor/IoMotorDriver.hpp"

BEGIN_IO_NAMESPACE

class MotorL293 : public MotorDriver {
  protected:
    IcL293& mIcL293;
    uint8_t mIndex;

  public:
    MotorL293(IcL293& icL293, uint8_t index = 0)
      : mIcL293(icL293)
      , mIndex(index) {}

    virtual ~MotorL293() {}

    inline void enable() {
      mIcL293.enable(mIndex);
    }

    inline void disable() {
      mIcL293.disable(mIndex);
    }

    inline void up(int speed = ANALOG_MAX) {
      mIcL293.enable(mIndex);
      mIcL293.up(mIndex, speed);
    }

    inline void down(int speed = ANALOG_MAX) {
      mIcL293.enable(mIndex);
      mIcL293.down(mIndex, speed);
    }

    inline void stop() {
      mIcL293.stop(mIndex);
    }

    inline void halt() {
      mIcL293.halt(mIndex);
    }
  };

END_IO_NAMESPACE
