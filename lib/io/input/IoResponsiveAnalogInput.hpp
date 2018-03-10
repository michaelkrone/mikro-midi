#pragma once

#include "IoInput.hpp"
#include "muxer/IoBaseMuxer.h"

BEGIN_IO_NAMESPACE

class ResponsiveAnalogInput : public Input {
  public:
    // pin - the pin to read
    // sleepEnable - enabling sleep will cause values to take less time to stop changing and potentially stop changing more abruptly,
    //   where as disabling sleep will cause values to ease into their correct position smoothly
    // snapMultiplier - a value from 0 to 1 that controls the amount of easing
    //   increase this to lessen the amount of easing (such as 0.1) and make the responsive values more responsive
    //   but doing so may cause more noise to seep through if sleep is not enabled

// @TODO make ioPins to IoInputs

  ResponsiveAnalogInput(
        ioPin pin, bool sleepEnable = false,
        float snapMultiplier = 0.1, int resolution = AnalogResolution)
    : ResponsiveAnalogInput(NULL, pin, sleepEnable, snapMultiplier) {}

  ResponsiveAnalogInput(
        Muxer* muxer, ioPin pin, bool sleepEnable = false,
        float snapMultiplier = 0.1, int resolution = AnalogResolution)
    : Input(muxer, pin, false, true)
    , mSleep(sleepEnable)
    , mResolution(resolution) {
      setSnapMultiplier(snapMultiplier);
    }

  virtual ~ResponsiveAnalogInput() {}

  void setSnapMultiplier(float multiplier) {
    if (multiplier > 1.0) {
      multiplier = 1.0;
    } else if (multiplier < 0.0) {
      multiplier = 0.0;
    }
    mMultiplier = multiplier;
  }

  inline int getValue() {
    return mValue;
  }

  inline int read() {
    return mValue;
  }

  inline int update() {
    update(this->Input::read());
    return mValue;
  }

 inline bool hasChanged() {
   return mResponsiveValueHasChanged;
 }

protected:
  bool mSleep;
  float mMultiplier;
  bool mEdgeSnapEnabled = true;
  bool mResponsiveValueHasChanged;
  bool mSleeping = false;
  float mActivityThreshold = 4.0;
  float mSmoothValue;
  float mErrorEMA = 0.0;
  int mPrevResponsiveValue;
  int mResolution;

  float snapCurve(float x) {
    float y = 1.0 / (x + 1.0);
    y = (1.0 - y) * 2.0;
    if (y > 1.0) {
      return 1.0;
    }
    return y;
  }

  int getResponsiveValue(int newValue) {
     // if sleep and edge snap are enabled and the new value is very close to an edge, drag it a little closer to the edges
     // This'll make it easier to pull the output values right to the extremes without mSleeping,
     // and it'll make movements right near the edge appear larger, making it easier to wake up
     if (mSleep && mEdgeSnapEnabled) {
       if (newValue < mActivityThreshold) {
         newValue = (newValue * 2) - mActivityThreshold;
       } else if (newValue > mResolution - mActivityThreshold) {
         newValue = (newValue * 2) - mResolution + mActivityThreshold;
       }
     }

     // get difference between new input value and current smooth value
     unsigned int diff = abs(newValue - mSmoothValue);

     // measure the difference between the new value and current value
     // and use another exponential moving average to work out what
     // the current margin of error is
     mErrorEMA += ((newValue - mSmoothValue) - mErrorEMA) * 0.4;

     // if sleep has been enabled, sleep when the amount of error is below the activity threshold
     if (mSleep) {
       // recalculate mSleeping status
       mSleeping = abs(mErrorEMA) < mActivityThreshold;
     }

     // if we're allowed to sleep, and we're mSleeping
     // then don't update mResponsiveValue this loop
     // just output the existing mResponsiveValue
     if (mSleep && mSleeping) {
       return (int)mSmoothValue;
     }

     // use a 'snap curve' function, where we pass in the diff (x) and get back a number from 0-1.
     // We want small values of x to result in an output close to zero, so when the smooth value is close to the input value
     // it'll smooth out noise aggressively by responding slowly to sudden changes.
     // We want a small increase in x to result in a much higher output value, so medium and large movements are snappy and responsive,
     // and aren't made sluggish by unnecessarily filtering out noise. A hyperbola (f(x) = 1/x) curve is used.
     // First x has an offset of 1 applied, so x = 0 now results in a value of 1 from the hyperbola function.
     // High values of x tend toward 0, but we want an output that begins at 0 and tends toward 1, so 1-y flips this up the right way.
     // Finally the result is multiplied by 2 and capped at a maximum of one, which means that at a certain point all larger movements are maximally snappy

     // then multiply the input by SNAP_MULTIPLER so input values fit the snap curve better.
     float snap = snapCurve(diff * mMultiplier);

     // when sleep is enabled, the emphasis is stopping on a mResponsiveValue quickly, and it's less about easing into position.
     // If sleep is enabled, add a small amount to snap so it'll tend to snap into a more accurate position before mSleeping starts.
     if (mSleep) {
       snap *= 0.5 + 0.5;
     }

     // calculate the exponential moving average based on the snap
     mSmoothValue += (newValue - mSmoothValue) * snap;

     // ensure output is in bounds
     if (mSmoothValue < 0.0) {
       mSmoothValue = 0.0;
     } else if (mSmoothValue > mResolution - 1) {
       mSmoothValue = mResolution - 1;
     }

     // expected output is an integer
     return (int)mSmoothValue;
   }

   inline void update(int value) {
     mPrevResponsiveValue = mValue;
     mValue = getResponsiveValue(value);
     mResponsiveValueHasChanged = mValue != mPrevResponsiveValue;
   }
};

END_IO_NAMESPACE
