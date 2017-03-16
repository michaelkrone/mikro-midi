#pragma once

#include "IoDefs.h"

BEGIN_IO_NAMESPACE

class SignalChangeCallback {
  protected:
    SignalEventType mType;
    SignalChangeHandler mHander;

  public:
    SignalChangeCallback()
      : mType(EventUninitialised) {}

  virtual ~SignalChangeCallback() {}

  inline SignalEventType getType() {
    return mType;
  }

  inline void setType(SignalEventType type) {
    mType = type;
  }

  inline void setHandler(SignalChangeHandler handler) {
    mHander = handler;
  }

  inline void callHandler(int value, SignalEventType type) {
    mHander(value, type);
  }
};

END_IO_NAMESPACE
