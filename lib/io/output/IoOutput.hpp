#pragma once

#include "IoPin.hpp"
#include "muxer/IoBaseMuxer.h"

BEGIN_IO_NAMESPACE

class Output : public Pin {
  private:
  Demuxer* mDemux;

  public:
  Output(ioPin pin, bool analog = false) : Output(NULL, pin, analog) {}

  Output(Demuxer* demuxer, ioPin pin, bool analog = false)
    : Pin(pin, false, analog)
    , mDemux(demuxer) {
      if (mDemux == NULL) {
        pinMode(mPin, OUTPUT);
      }
    }

  virtual ~Output() {}

  inline void write(int value) {
    if (mDemux != NULL) {
      return mDemux->writePin(mPin, value);
    }

    if (!mAnalog) {
      return digitalWriteFast(mPin, value ? HIGH : LOW);
    }

    analogWrite(mPin, value);
  }
};

END_IO_NAMESPACE
