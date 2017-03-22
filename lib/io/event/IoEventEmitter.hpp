
#pragma once

#include "IoDefs.h"
#include "IoSignalChangeCallback.hpp"
#include "IoBouncedInput.hpp"

BEGIN_IO_NAMESPACE

class EventEmitter {
  private:
    BouncedInput& mInput;
    SignalChangeCallback mListeners[MaxSignalEventListeners];

  public:
  EventEmitter(BouncedInput& input)
    : mInput(input) {}

  virtual ~EventEmitter() {}

  inline BouncedInput getInput() {
    return mInput;
  }

  inline SignalChangeCallback* addListener(
      SignalEventType type, SignalChangeHandler handler, int args = 0) {
    SignalChangeCallback* next = nextCallback();
		if (!next) {
      return NULL;
    }

  	next->setType(type);
  	next->setHandler(handler);
  	return next;
  }

  bool update() {
    bool updated = mInput->update();
    if (updated) {
      if (mInput->up()) {
        fireEvent(EventUp);
      } else if (mInput->down()) {
        fireEvent(EventDown);
      }
    }
    return updated;
  }

  private:
    inline void fireEvent(SignalEventType eventType) {
      int value = mInput->getValue();
      for (uint8_t i = 0; i < MaxSignalEventListeners; i++) {
        SignalEventType t = mListeners[i].getType();
        if (t == eventType || t == EventCatchAll) {
          mListeners[i].callHandler(value, eventType);
        }
      }
    }

    inline SignalChangeCallback* nextCallback() {
      for (uint8_t i = 0; i < MaxSignalEventListeners; i++) {
        if(mListeners[i].getType() == EventUninitialised){
          return &mListeners[i];
        }
      }
	    return NULL;
    }
};

END_IO_NAMESPACE
