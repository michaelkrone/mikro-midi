#pragma once

#include "IoDefs.h"
#include "IoIc.hpp"

BEGIN_IO_NAMESPACE

class Ic595 : public Ic {
  protected:
    ioPin mPinShcp;
    ioPin mPinStcp;
    ioPin mEnablePin;

  public:
    uint8_t mNumIcs;
    static const uint8_t NUM_CHANNELS = 8;
    ioPin mPinMux;
    uint8_t mChannels;

    Ic595(ioPin pinSHCP, ioPin pinSTCP, ioPin pinDS, uint8_t numIcs = 1, ioPin enablePin = NoCurrentPin)
        : mPinShcp(pinSHCP), mPinStcp(pinSTCP), mEnablePin(enablePin)
        , mNumIcs(numIcs), mPinMux(pinDS), mChannels(mNumIcs * NUM_CHANNELS) {
        // define pins as outputs
        pinMode(mPinShcp, OUTPUT);
        pinMode(mPinStcp, OUTPUT);
        pinMode(mPinMux, OUTPUT);

        // set pins low
        digitalWriteFast(mPinShcp, LOW);
        digitalWriteFast(mPinStcp, LOW);
        digitalWriteFast(mPinMux, LOW);
    }

    virtual ~Ic595() {}

    inline void enable() {
      setEnableState(true);
    }

    inline void disable() {
      setEnableState(false);
    }

    inline void setEnableState(bool enabled) {
      if (mEnablePin != NoCurrentPin) {
        digitalWriteFast(mEnablePin, enabled ? LOW : HIGH);
      }
    }

    inline void writeAll(uint8_t* values) {
      for (uint8_t i = mNumIcs; i-- > 0;) {
        shiftOut(mPinMux, mPinShcp, MSBFIRST, values[i]);
      }
      digitalWriteFast(mPinStcp, HIGH);
      digitalWriteFast(mPinStcp, LOW);
    }
};

END_IO_NAMESPACE
