#pragma once

#include "IoPin.hpp"
#include "muxer/IoBaseMuxer.h"

BEGIN_IO_NAMESPACE

class Input : public Pin {
  protected:
    Muxer* mMux;
    int mValue;

  public:
  Input(ioPin pin, bool pullup = true, bool analog = false)
    : Input(NULL, pin, pullup, analog) {}

  Input(Muxer* muxer, ioPin pin, bool pullup = true, bool analog = false)
    : Pin(pin, pullup, analog)
    , mMux(muxer)
    , mValue(0) {
      if (mMux == NULL) {
        pinMode(mPin, mPullup ? INPUT_PULLUP : INPUT);
      }
    }

  virtual ~Input() {}

  inline bool isPullup() {
    return mPullup;
  }

  inline int getValue() {
    return mValue;
  }

  inline int read() {
    if (mMux != NULL) {
      mValue = mMux->readPin(mPin);
    } else if (!mAnalog) {
      mValue = DIGITAL_READ_PULLUP(digitalReadFast(mPin), mPullup);
    } else {
      mValue = analogRead(mPin);
    }
    return mValue;
  }
};

END_IO_NAMESPACE
