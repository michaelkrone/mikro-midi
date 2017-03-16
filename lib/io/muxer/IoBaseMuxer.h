#pragma once

#include "IoDefs.h"

BEGIN_IO_NAMESPACE

class BaseMuxer {
  protected:
  uint8_t mChannels;
  bool mAnalog;
  // this is an uint8_t since it describes the pin on the chip itself
  uint8_t mCurrentPin;
  int* mValues;
  uint32_t mDelay;
  bool mTouch;

  public:
    BaseMuxer(uint8_t numChannels, bool analog, uint32_t delay, bool touch)
      : mChannels(numChannels)
      , mAnalog(analog)
      , mCurrentPin(NoCurrentPin)
      , mDelay(delay)
      , mTouch(touch) {
      // allocates the specified number of bytes and initializes them to zero
      int len = mChannels * sizeof(int);
      mValues = (int*) malloc(len);
      memset(mValues, 0, len);
  }

  virtual ~BaseMuxer() {}

  bool isAnalog(void) {
    return mAnalog;
  }

  bool isTouch(void) {
    return mTouch;
  }

  void setTouch(bool touch) {
    mTouch = touch;
  }

  inline uint8_t getChannelLength() {
    return mChannels;
  }

  inline int* getValues() {
    return mValues;
  }

  inline ioPin getCurrentPin() {
    return mCurrentPin;
  }

  inline void selectPin(ioPin pin);
};

class Muxer : public BaseMuxer {
  public:
  Muxer(uint8_t numChannels, bool analog = false, uint32_t delay = 0, bool touch = false)
    : BaseMuxer(numChannels, analog, delay, touch) {}
  virtual int readPin(ioPin pin);
  inline int readPin();
  inline int* readAll();
};

class Demuxer : public BaseMuxer {
  public:
  Demuxer(uint8_t numChannels, bool analog = false, uint32_t delay = 0)
    : BaseMuxer(numChannels, analog, delay, false) {}
  virtual void writePin(ioPin pin, int value);
  virtual void setPin(ioPin pin, int value, bool update);
  inline void writeAll(int values);
  inline void writeAll();
};

END_IO_NAMESPACE
