#pragma once

#include <Throttle.hpp>

#include "IoDefs.h"
#include "IoResponsiveAnalogInput.hpp"
#include "ic/IoIcL293.hpp"

BEGIN_IO_NAMESPACE

class MotorFader {
  private:
    ResponsiveAnalogInput& mInput;
    IcL293& mIc;
    uint8_t mIcIndex;
    int mTarget;
    uint16_t mMin, mMax;
    float mMinState, mMaxState;
    throttle::Throttle mThrottle;
    uint16_t mMaxResolution;
    float mPterm, mDterm, mIterm;
    float mPgain, mIgain, mDgain;
    float mDstate, mPlant;

    inline void calculateDrive(float faderError, float faderPosition) {
      // calculate the proportional term
      mPterm = mPgain * faderError;

      // calculate the integral state with appropriate limiting
      mIterm = mIgain * faderError;
      if (mIterm < mMinState) {
        mIterm = mMinState;
      } else if (mIterm > mMaxState) {
        mIterm = mMaxState;
      }

      mDterm = mDgain * (faderPosition - mDstate);
      mDstate = faderPosition;
      mPlant = mPterm + mIterm - mDterm;

      if (mPlant > mMaxResolution) {
        mPlant = mMaxResolution;
      } else if (mPlant < -mMaxResolution) {
        mPlant = -mMaxResolution;
      }
    }

  public:
    MotorFader(
        ResponsiveAnalogInput& faderLevel,
        IcL293& icL293, uint8_t icIndex = 0,
        uint8_t defaultTimeoutMs = 1, uint16_t analogResolution = 255,
        uint16_t minFaderValue = DefaultFaderMin, uint16_t maxFaderValue = DefaultFaderMax,
        float gainP = DefaultGainP, float gainI = DefaultGainI, float gainD = DefaultGainD)
    : mInput(faderLevel)
    , mIc(icL293)
    , mIcIndex(icIndex)
    , mMin(minFaderValue)
    , mMax(maxFaderValue)
    , mThrottle(defaultTimeoutMs)
    , mMaxResolution(analogResolution)
    , mPgain(gainP)
    , mIgain(gainI)
    , mDgain(gainD) {
      mTarget = 0.0;
      mPlant = 0.0;
      mMinState = -1500;
      mMaxState = 1500;
    }

    virtual ~MotorFader() {}

    inline void set(float value) {
      if (value > mMax) {
        mTarget = mMax;
      } else if (value < mMin) {
        mTarget = mMin;
      } else {
        mTarget = (int)value;
      }
      mThrottle.invalidate();
    }

    /**
    Non-Blocking move function, moves the fader to a level
    */
    inline void update() {
      if (!mThrottle.shouldUpdate()) {
        return;
      }

      // update plant value
      int faderValue = mInput.update();
      calculateDrive(mTarget - faderValue, faderValue);

      mThrottle.reset(false);

      if (!mIc.mEnabled[mIcIndex]) {
        mIc.enable(mIcIndex);
      }

      if (mPlant > 0 && faderValue < mMax){
        mIc.up(mIcIndex, mPlant);
      } else if (mPlant < 0 && faderValue > mMin){
        mIc.down(mIcIndex, -mPlant);
      } else {
        mIc.stop(mIcIndex);
        mIc.disable(mIcIndex);
        mThrottle.validate();
      }
    }
};

END_IO_NAMESPACE
