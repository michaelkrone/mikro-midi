#pragma once

#include "IoDefs.h"
#include "IoIc.hpp"
#include "output/IoOutput.hpp"

BEGIN_IO_NAMESPACE

static const uint8_t MotorNb = 2;

class IcL293 : public Ic {
  protected:
    Output* mEnable[MotorNb];
    Output* mDriverInput[MotorNb][2];
    bool mEnabled[2];

  public:
    IcL293(Output* enableA, Output* driverInputA1, Output* driverInputA2,
        Output* enableB = NULL, Output* driverInputB1 = NULL, Output* driverInputB2 = NULL) {
      mEnable[0] = enableA;
      mEnable[1] = enableB;
      mDriverInput[0][0] = driverInputA1;
      mDriverInput[0][1] = driverInputA2;
      mDriverInput[1][0] = driverInputB1;
      mDriverInput[1][1] = driverInputB2;
      mEnabled[0] = false;
      mEnabled[1] = false;
    }

    virtual ~IcL293() {}

    inline void enable(uint8_t index = 0) {
      if (mEnable[index] != NULL && !mEnabled[index]) {
        mEnable[index]->write(ANALOG_MAX);
        mEnabled[index] = true;
      }
    }

    inline void disable(uint8_t index = 0) {
      if (mEnable[index] != NULL && mEnabled[index]) {
        mEnable[index]->write(0);
        mEnabled[index] = false;
      }
    }

    inline void up(uint8_t index = 0, int value = ANALOG_MAX) {
      if (mDriverInput[index][0] != NULL) {
        mDriverInput[index][1]->write(0);
        mDriverInput[index][0]->write(value);
      }
    }

    inline void down(uint8_t index = 0, int value = ANALOG_MAX) {
      if (mDriverInput[index][0] != NULL) {
        mDriverInput[index][0]->write(0);
        mDriverInput[index][1]->write(value);
      }
    }

    inline void stop(uint8_t index = 0) {
      if (mDriverInput[index][0] != NULL) {
        mDriverInput[index][0]->write(0);
        mDriverInput[index][1]->write(0);
      }
    }

    inline void halt(uint8_t index = 0) {
      if (mDriverInput[index][0] != NULL) {
        mDriverInput[index][0]->write(1);
        mDriverInput[index][1]->write(1);
      }
    }

    inline void stopAll() {
      for (uint8_t i = 0; i < MotorNb; i++) {
        enable(i);
        stop(i);
        disable(i);
      }
    }

    inline void haltAll() {
      for (uint8_t i = 0; i < MotorNb; i++) {
        enable(i);
        halt(i);
        disable(i);
      }
    }
  };

END_IO_NAMESPACE
