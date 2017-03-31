#pragma once

#include "IoBaseMuxer.h"
#include "ic/IoIc595.hpp"

BEGIN_IO_NAMESPACE

class Demuxer595 : public Demuxer {
  public:
    Ic595& mIc595;

  private:
    bool mAutoWrite;
    bool mChanged;
    uint8_t* mBytes;

  public:
    Demuxer595(Ic595& ic595, uint32_t writeDelay = 0, bool autoWrite = true)
      : Demuxer(ic595.mNumIcs, false, 0), mIc595(ic595), mAutoWrite(autoWrite), mChanged(false) {
        int len = (mIc595.mNumIcs * Ic595::NUM_CHANNELS) * sizeof(uint8_t);
        mBytes = (uint8_t*) malloc(len);
        memset(mBytes, 0, len);
      }

    virtual ~Demuxer595() {}

    inline int readPin(ioPin pin) {
      return (mValues[pin / Ic595::NUM_CHANNELS] >> (pin % Ic595::NUM_CHANNELS)) & 1;
    }

    void setAutoWrite(bool autoWrite) {
      mAutoWrite = autoWrite;
    }

    inline void writePin(ioPin pin, int value) {
      setPin(pin, value, mAutoWrite);
    }

    inline void setPin(ioPin pin, int value, bool write = true) {
      mChanged = true;
      if (value) {
         mValues[pin / Ic595::NUM_CHANNELS] |= 1 << (pin % Ic595::NUM_CHANNELS);
      } else {
         mValues[pin / Ic595::NUM_CHANNELS] &= ~(1 << (pin % Ic595::NUM_CHANNELS));
      }

      mCurrentPin = pin;

      if (write) {
        writeAll();
      }
    }

    /*
    values might be an int array like [0, 1, 0, 127, 0, 127, 0, 1]
    we need this to be a single byte like B01010101
    */
    inline void writeAll(const int* values, uint8_t length = 0, bool write = true) {
      if (length == 0) {
        length = mIc595.mNumIcs * Ic595::NUM_CHANNELS;
      }

      for (uint8_t i = 0; i < length; i++) {
        setPin(i, values[i], false);
      }

      if (write) {
        writeAll();
      }
    }

    inline void writeAll() {
      if (!mChanged) {
        return;
      }

      mIc595.writeAll(mValues);
    }

    inline void update() {
      writeAll();
    }
};

END_IO_NAMESPACE
