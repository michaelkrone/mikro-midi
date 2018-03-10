#pragma once

#include "IoDefs.h"
#include "IoIc.hpp"

BEGIN_IO_NAMESPACE

class Ic4051 : public Ic {
  protected:
  ioPin mPinS0;
  ioPin mPinS1;
  ioPin mPinS2;
  uint32_t mBounceDelay;
  ioPin mEnablePin;

  public:
  uint8_t mChannels;
  ioPin mPinMux;

// @TODO make ioPins to IoOutputs

  Ic4051(ioPin pinS0, ioPin pinS1, ioPin pinS2, ioPin pinMux, uint8_t numChannels = 8,
        uint32_t bounceDelay = DefaultBounceDelayMicros_4051, ioPin enablePin = NoCurrentPin)
      : mPinS0(pinS0), mPinS1(pinS1), mPinS2(pinS2), mBounceDelay(bounceDelay)
      , mEnablePin(enablePin), mChannels(numChannels), mPinMux(pinMux) {
      // define pins as outputs
      pinMode(mPinS0, OUTPUT);
      pinMode(mPinS1, OUTPUT);
      pinMode(mPinS2, OUTPUT);

      // set pins low
      digitalWriteFast(mPinS0, LOW);
      digitalWriteFast(mPinS1, LOW);
      digitalWriteFast(mPinS2, LOW);
  }

  virtual ~Ic4051() {}

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

  inline bool selectPin(ioPin pin) {
    if (pin >= mChannels || pin == NoCurrentPin) {
      return false;
    }

    digitalWriteFast(mPinS0, (pin &  0x01));
    digitalWriteFast(mPinS1, (pin &  0x02));
    digitalWriteFast(mPinS2, (pin &  0x04));
    // wait for the signal to get stable
    delayMicroseconds(mBounceDelay);
    return true;
  }
};

END_IO_NAMESPACE
