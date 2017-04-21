#pragma once

#include <Throttle.hpp>

#include "IoDefs.h"
#include "input/IoResponsiveAnalogInput.hpp"
#include "motor/IoMotorDriver.hpp"
#include "ic/IoIcL293.hpp"

BEGIN_IO_NAMESPACE

class PidController {
  private:
    ResponsiveAnalogInput& mInput;
    MotorDriver& mIc;
    int mTarget;
    uint16_t mMin, mMax;
    float mMinState, mMaxState;
    throttle::Throttle mThrottle;
    uint16_t mAnalogMax;
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

      if (mPlant > mAnalogMax) {
        mPlant = mAnalogMax;
      } else if (mPlant < -mAnalogMax) {
        mPlant = -mAnalogMax;
      }
    }

  public:
    PidController(
        ResponsiveAnalogInput& faderLevel,
        MotorDriver& driver,
        uint8_t defaultTimeoutMs = 10, uint16_t analogMax = ANALOG_MAX,
        uint16_t minFaderValue = DefaultFaderMin, uint16_t maxFaderValue = DefaultFaderMax,
        float gainP = DefaultGainP, float gainI = DefaultGainI, float gainD = DefaultGainD)
    : mInput(faderLevel)
    , mIc(driver)
    , mMin(minFaderValue)
    , mMax(maxFaderValue)
    , mThrottle(defaultTimeoutMs)
    , mAnalogMax(analogMax)
    , mPgain(gainP)
    , mIgain(gainI)
    , mDgain(gainD) {
      mTarget = 0.0;
      mPlant = 0.0;
      mMinState = -1500;
      mMaxState = 1500;
    }

    virtual ~PidController() {}

    inline void setValue(float value) {
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

      mIc.enable();
      if (mPlant > 0 && faderValue < mMax) {
        digitalWrite(13, LOW);
        mIc.up(mPlant);
      } else if (mPlant < 0 && faderValue > mMin) {
        mIc.down(-mPlant);
      } else {
        mIc.stop();
        mIc.disable();
        mThrottle.validate();
      }
    }
};

END_IO_NAMESPACE
