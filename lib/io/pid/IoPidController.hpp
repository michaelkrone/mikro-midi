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
    throttle::Throttle mThrottle;
    uint16_t mAnalogMax;
    float mPterm, mDterm, mIterm;
    float mPgain, mIgain, mDgain;
    float mMinState, mMaxState;
    float mDstate, mIstate, mPlant;

    inline void updatePIDIntegralState(float error, float position) {
      // calculate the proportional term
      mPterm = mPgain * error;
      mIstate += error;

      // calc ulate the integral state with appropriate limiting
      if (mIstate > mMaxState) {
        mIstate = mMaxState;
      } else if (mIstate < mMinState) {
        mIstate = mMinState;
      }

      mIterm = mIgain * mIstate;
      mDterm = mDgain * (position - abs(mDstate));
      mDstate = position;
      mPlant = mPterm + mIterm - mDterm;

      if (mPlant > mAnalogMax) {
        mPlant = mAnalogMax;
      } else if (mPlant < -mAnalogMax) {
        mPlant = -mAnalogMax;
      }
    }

    inline void updatePID(float error, float position) {
      updatePIDIntegralState(error, position);
    }

  public:
    PidController(
        ResponsiveAnalogInput& faderLevel,
        MotorDriver& driver,
        uint16_t minFaderValue = DefaultFaderMin, uint16_t maxFaderValue = DefaultFaderMax,
        uint8_t defaultTimeoutMs = 5, uint16_t analogMax = DefaultAnalogMax,
        float gainP = DefaultGainP, float gainI = DefaultGainI, float gainD = DefaultGainD,
        float minIntegralState = -900000, float maxIntegralState = 900000)
    : mInput(faderLevel)
    , mIc(driver)
    , mMin(minFaderValue)
    , mMax(maxFaderValue)
    , mThrottle(defaultTimeoutMs)
    , mAnalogMax(analogMax)
    , mPgain(gainP)
    , mIgain(gainI)
    , mDgain(gainD)
    , mMinState (minIntegralState)
    , mMaxState (maxIntegralState)
    {
      mTarget = 0.0;
      mPlant = 0.0;
      mDstate = 0.0;
      mIstate = 0.0;
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
      mIstate = 0;
      mThrottle.invalidate();
    }

    /**
    Non-Blocking move function, moves the fader to a level
    */
    inline void update() {
      if (!mThrottle.shouldUpdate()) {
        return;
      }

      mThrottle.reset(false);

      // update plant value
      int faderValue = mInput.update();
      float error = (float) (mTarget - faderValue);

      if (error == 0) {
        stop();
        return;
      }

      updatePID(error, faderValue);
      mIc.enable();

      if (mPlant > 0 && faderValue < mMax) {
        mIc.up(mPlant);
      } else if (mPlant < 0 && faderValue > mMin) {
        mIc.down(-mPlant);
      } else {
        stop();
      }
    }

    inline void stop() {
      mIc.stop();
      mIc.disable();
      mThrottle.validate();
    }
};

END_IO_NAMESPACE
