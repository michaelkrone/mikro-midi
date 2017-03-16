#pragma once

#include "IoDefs.h"
#include "IoOutput.hpp"
#include "IoIc.hpp"

BEGIN_IO_NAMESPACE

static const uint8_t MotorNb = 2;

class IcL293 : public Ic {
  protected:
    Output* mEnable[MotorNb];
    Output* mDriverInput[MotorNb][2];


  public:
    bool mEnabled[2];

    IcL293(Output* enableA12, Output* driverInputA1, Output* driverInputA2,
        Output* enableA34 = NULL, Output* driverInputA3 = NULL, Output* driverInputA4 = NULL) {
      mEnable[0] = enableA12;
      mEnable[1] = enableA34;
      mDriverInput[0][0] = driverInputA1;
      mDriverInput[0][1] = driverInputA2;
      mDriverInput[1][0] = driverInputA3;
      mDriverInput[1][1] = driverInputA4;
      mEnabled[0] = false;
      mEnabled[1] = false;
    }

    virtual ~IcL293() {}

    inline void enable(uint8_t index = 0, int speed = 255) {
      if (mEnable[index] != NULL) {
        mEnable[index]->write(speed);
        mEnabled[index] = true;
      }
    }

    inline void disable(uint8_t index = 0) {
      if (mEnable[index] != NULL) {
        mEnable[index]->write(0);
        mEnabled[index] = false;
      }
    }

    inline void setEnabled(uint8_t index, bool enabled) {
      if (enabled) {
        return enable(index);
      }
      disable(index);
    }

    inline void up(uint8_t index = 0, int value = 255) {
      if (mDriverInput[index][0] != NULL) {
        mDriverInput[index][1]->write(0);
        mDriverInput[index][0]->write(value);
      }
    }

    inline void down(uint8_t index = 0, int value = 255) {
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
