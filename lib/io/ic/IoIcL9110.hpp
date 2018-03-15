#pragma once

#include "IoDefs.h"
#include "IoIc.hpp"
#include "output/IoOutput.hpp"

BEGIN_IO_NAMESPACE

static const uint8_t IcL9110MotorNb = 2;

class IcL9110 : public Ic {
  protected:
    Output* mSpeed[IcL9110MotorNb];
    Output* mDirection[IcL9110MotorNb];

  public:
    IcL9110(
      Output* speedA, Output* directionA,
      Output* speedB = NULL, Output* directionB = NULL
    ) {
      mSpeed[0] = speedA;
      mSpeed[1] = speedB;
      mDirection[0] = directionA;
      mDirection[1] = directionB;
      stopAll();
    }

    virtual ~IcL9110() {}

    inline void up(uint8_t index = 0, int speed = ANALOG_MAX) {
      if (index >= IcL9110MotorNb || mDirection[index] == NULL) {
        return;
      }
      stop(index);
      mDirection[index]->write(speed);
    }

    inline void down(uint8_t index = 0, int speed = ANALOG_MAX) {
      if (index >= IcL9110MotorNb || mSpeed[index] == NULL) {
        return;
      }
      stop(index);
      mSpeed[index]->write(speed);
    }

    inline void stop(uint8_t index = 0) {
      if (index >= IcL9110MotorNb || mSpeed[index] == NULL) {
        return;
      }
      mSpeed[index]->write(LOW);
      mDirection[index]->write(LOW);
    }

    inline void halt(uint8_t index = 0) {
      if (index >= IcL9110MotorNb || mSpeed[index] == NULL) {
        return;
      }
      mSpeed[index]->write(ANALOG_MAX);
      mDirection[index]->write(ANALOG_MAX);
    }

    inline void stopAll() {
      for (uint8_t i = 0; i < IcL9110MotorNb; i++) {
        stop(i);
      }
    }

    inline void haltAll() {
      for (uint8_t i = 0; i < IcL9110MotorNb; i++) {
        halt(i);
      }
    }
  };

END_IO_NAMESPACE
