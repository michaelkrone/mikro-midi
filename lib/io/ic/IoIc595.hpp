#pragma once

#include "IoDefs.h"
#include "IoIc.hpp"
#include "output/IoOutput.hpp"

BEGIN_IO_NAMESPACE

class Ic595 : public Ic {
  private:
    bool mEnabled;
  protected:
    ioPin mPinShcp;
    ioPin mPinStcp;
    io::Output* mEnable;

  public:
    uint8_t mNumIcs;
    static const uint8_t NUM_CHANNELS = 8;
    ioPin mPinMux;
    uint8_t mChannels;

    // SHCP or SRCK
    // STCP or RCK
    // DS or SER IN
    // enableOutput: OE or G
    Ic595(ioPin pinSHCP, ioPin pinSTCP, ioPin pinDS, uint8_t numIcs = 1, io::Output* enableOutput = NULL)
         : mPinShcp(pinSHCP), mPinStcp(pinSTCP), mEnable(enableOutput)
        , mNumIcs(numIcs), mPinMux(pinDS), mChannels(mNumIcs * NUM_CHANNELS) {
        // define pins as outputs
        pinMode(mPinShcp, OUTPUT);
        pinMode(mPinStcp, OUTPUT);
        pinMode(mPinMux, OUTPUT);

        // set pins low
        digitalWriteFast(mPinShcp, LOW);
        digitalWriteFast(mPinStcp, LOW);
        digitalWriteFast(mPinMux, LOW);
        disable();
    }

    virtual ~Ic595() {}

    inline void enable() {
      setEnableState(true);
    }

    inline void disable() {
      setEnableState(false);
    }

    inline void setEnableState(bool enabled) {
      if (mEnabled && mEnable != NULL) {
        mEnable->write(enabled ? LOW : HIGH);
        mEnabled = enabled;
      }
    }

    inline void writeAll(int* values) {
      for (uint8_t i = mNumIcs; i-- > 0;) {
        shiftOut(mPinMux, mPinShcp, MSBFIRST, values[i]);
      }
      digitalWriteFast(mPinStcp, HIGH);
      digitalWriteFast(mPinStcp, LOW);
    }
};

END_IO_NAMESPACE
