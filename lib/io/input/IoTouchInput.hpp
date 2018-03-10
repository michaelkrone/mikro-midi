#pragma once

#include "IoResponsiveAnalogInput.hpp"
#include "muxer/IoBaseMuxer.h"

BEGIN_IO_NAMESPACE

static const int8_t NoTouch = -1;

class TouchInput : public ResponsiveAnalogInput {
  private:
    uint16_t mCapSense;
    int mTouchVal;

// @TODO make ioPins to IoInputs

  public:
  TouchInput(ioPin pin, uint16_t capsense = DefaultCapsense)
    : TouchInput(NULL, pin) {}

  TouchInput(Muxer* muxer, ioPin pin, uint16_t capsense = DefaultCapsense)
    : ResponsiveAnalogInput(muxer, pin, true, 0.1, 16000)
    , mCapSense(capsense), mTouchVal(NoTouch) {
      this->ResponsiveAnalogInput::update(read());
    }

  virtual ~TouchInput() {}

  inline int update() {
    int prev = mValue;
    this->ResponsiveAnalogInput::update(read());

    if (mTouchVal != NoTouch && mTouchVal - mValue <= mCapSense) {
      mTouchVal = mValue;
    } else if (mValue - prev >= mCapSense) {
      mTouchVal = mValue;
    } else {
      mTouchVal = NoTouch;
    }

    return mValue;
  }

  inline bool isTouched() {
    return mTouchVal != NoTouch;
  }

  inline int read() {
    if (mMux != NULL) {
      mMux->setTouch(true);
      mValue = mMux->readPin(mPin);
      mMux->setTouch(false);
    } else {
      mValue = touchRead(mPin);
    }

    return mValue;
  }
};

END_IO_NAMESPACE
