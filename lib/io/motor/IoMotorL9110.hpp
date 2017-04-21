#pragma once

#include "IoDefs.h"
#include "output/IoOutput.hpp"
#include "motor/IoMotorDriver.hpp"

BEGIN_IO_NAMESPACE

class MotorL9110 : public MotorDriver {
  protected:
    Output& mSpeed;
    Output& mDirection;

  public:
    MotorL9110(Output& speed, Output& direction)
      : mSpeed(speed)
      , mDirection(direction) {}

    virtual ~MotorL9110() {}

    inline void enable() {}

    inline void disable() {}

    inline void up(int speed = ANALOG_MAX) {
      stop();
      mDirection.write(speed);
    }

    inline void down(int speed = ANALOG_MAX) {
      stop();
      mSpeed.write(speed);
    }

    inline void stop() {
      mSpeed.write(LOW);
      mDirection.write(LOW);
    }

    inline void halt() {
      mSpeed.write(ANALOG_MAX);
      mDirection.write(ANALOG_MAX);
    }
  };

END_IO_NAMESPACE
