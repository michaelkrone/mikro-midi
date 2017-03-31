#pragma once

#include "IoDefs.h"

#include <IoPin.hpp>
#include <input/IoInput.hpp>

BEGIN_IO_NAMESPACE

class RotaryEncoder {
  private:
    io::Input& mPin1;
    io::Input& mPin2;
    uint8_t mState;
    int mPrevValue;

    inline void updateState(void) {
      mPrevValue = mValue;
  		uint8_t s = mState & 3;

      if (mPin1->read()) {
        s |= 4;
      }

  		if (mPin2->read()) {
        s |= 8;
      }

  		switch (s) {
  			case 0: case 5: case 10: case 15:
  				break;

        case 1: case 7: case 8: case 14: {
  				mValue++;
          break;
        }

  			case 2: case 4: case 11: case 13: {
  				mValue--;
          break;
        }

  			case 3: case 12: {
  				mValue += 2;
          break;
        }

  			default: {
  				mValue -= 2;
          break;
        }
  		}

  		mState = (s >> 2);
  	}

  protected:
    int mValue;

  public:
  RotaryEncoder(io::Input& pin1, io::Input& pin2) : mPin1(pin1), mPin2(pin2) {}

  virtual ~RotaryEncoder() {}

  inline int getValue() {
    return mValue;
  }

  inline int read() {
    updateState();
    return mValue;
  }

  inline bool cw() {
    return mPrevValue < mValue;
  }
  inline bool ccw() {
    return mPrevValue > mValue;
  }
};

END_IO_NAMESPACE
