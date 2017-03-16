#pragma once

#include "IoBaseMuxer.h"
#include "ic/IoIc4051.hpp"

BEGIN_IO_NAMESPACE

class Muxer4051 : public Muxer {
  private:
    bool mPullup;
    Ic4051& mIc4051;

    inline int readMux(ioPin pin) {
      int value;

      mIc4051.selectPin(pin);
      mCurrentPin = pin;

      if (!mAnalog) {
        value = DIGITAL_READ_PULLUP(digitalReadFast(mIc4051.mPinMux), mPullup);
      } else if (mTouch) {
        value = touchRead(mIc4051.mPinMux);
      } else {
        value = analogRead(mIc4051.mPinMux);
      }

      mValues[pin] = value;
      return value;
    }

  public:
    Muxer4051(Ic4051& ic4051, bool analog = false, bool pullup = true, bool touch = false)
      : Muxer(ic4051.mChannels, analog)
      , mPullup(pullup)
      , mIc4051(ic4051) {
      pinMode(mIc4051.mPinMux, mPullup ? INPUT_PULLUP : INPUT);
    }

    virtual ~Muxer4051() {}

    inline bool isPullup() {
      return mPullup;
    }

    inline int readPin(ioPin pin) {
      return readMux(pin);
    }

    inline int readPin() {
      return readMux(mCurrentPin);
    }

    inline int* readAll() {
      for (ioPin pin = 0; pin < mIc4051.mChannels; pin++) {
        readMux(pin);
      }
      return mValues;
    }
};

END_IO_NAMESPACE
