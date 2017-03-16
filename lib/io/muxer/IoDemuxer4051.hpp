#pragma once

#include "IoBaseMuxer.h"
#include "ic/IoIc4051.hpp"

BEGIN_IO_NAMESPACE

class Demuxer4051 : public Demuxer {
  public:
  Demuxer4051(Ic4051& ic4051, bool analog = false, uint32_t writeDelay = DefaultDemuxDelayMicros_4051)
    : Demuxer(ic4051.mChannels, analog, writeDelay)
    , mIc4051(ic4051) {
    pinMode(mIc4051.mPinMux, OUTPUT);
  }

  virtual ~Demuxer4051() {}

  inline void setPin(ioPin pin, int value, bool update = true) {
    writeDemux(pin, value);
  }

  inline void writePin(ioPin pin, int value) {
    writeDemux(pin, value);
  }

  inline void writePin(int value) {
    writeDemux(mCurrentPin, value);
  }

  inline void writeAll(int* values) {
    for (ioPin pin = 0; pin < mIc4051.mChannels; pin++) {
      writeDemux(pin, values[pin]);
    }
  }

  inline void writeAll() {
    writeAll(mValues);
  }

  private:
    Ic4051& mIc4051;

  inline void writeDemux(ioPin pin, int value) {
    mIc4051.selectPin(pin);

    if (!mAnalog) {
      digitalWriteFast(mIc4051.mPinMux, value ? HIGH : LOW);
    } else {
      analogWrite(mIc4051.mPinMux, value);
    }

    mCurrentPin = pin;
    mValues[pin] = value;
    delayMicroseconds(mDelay);
  }
};

END_IO_NAMESPACE
