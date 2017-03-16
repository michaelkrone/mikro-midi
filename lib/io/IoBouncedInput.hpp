
#pragma once

#include "IoDefs.h"
#include "IoInput.hpp"

BEGIN_IO_NAMESPACE

class BouncedInput : public Input {
  public:
  BouncedInput(ioPin pin, bool pullup = true,
        unsigned long interval = BounceIntervalDefault, DebounceMode bounceMode = DebounceModeStable)
    : BouncedInput(NULL, pin, pullup, interval, bounceMode) {}

  BouncedInput(Muxer* muxer, ioPin pin, bool pullup = true,
        unsigned long interval = BounceIntervalDefault, DebounceMode bounceMode = DebounceModeStable)
    : Input(muxer, pin, pullup, false)
    , mIntervalTime(interval)
    , mMode(bounceMode)
    , mPrevTime(0)
    , mState(0) {
    if (this->Input::read()) {
      mState = MASK_BOUNCE_STATE(DebouncedState) | MASK_BOUNCE_STATE(DebouncedStateUnstable);
    }
    mPrevTime = (mMode == DebounceModeLockout) ? 0 : millis();
  }

  virtual ~BouncedInput() {}

  bool update() {
    switch (mMode) {
      case DebounceModeLockout: {
        return bounceLockout();
      }

      case DebounceModePromptDetection: {
        return bouncePromtDetection();
      }

      case DebounceModeStable:
      default: {
        return bounceStable();
      }
    }
  }

  inline bool getState() {
    return mState & MASK_BOUNCE_STATE(DebouncedState);
  }

  inline bool up() {
    return (mState & MASK_BOUNCE_STATE(DebouncedState)) && (mState & MASK_BOUNCE_STATE(DebouncedStateChanged));
  }

  inline bool down() {
    return !(mState & MASK_BOUNCE_STATE(DebouncedState)) && (mState & MASK_BOUNCE_STATE(DebouncedStateChanged));
  }

  unsigned long getLastEventTime() {
    return mPrevTime;
  }

  protected:
    uint16_t mIntervalTime;
    DebounceMode mMode;
    unsigned long mPrevTime;
    DebounceState mState;

  inline bool bounceLockout() {
    mState &= ~MASK_BOUNCE_STATE(DebouncedStateChanged);
    // Ignore everything if we are locked out
    if (millis() - mPrevTime >= mIntervalTime) {
      bool currentState = this->Input::read();
      if ((bool)(mState & MASK_BOUNCE_STATE(DebouncedState)) != currentState) {
        mPrevTime = millis();
        mState ^= MASK_BOUNCE_STATE(DebouncedState);
        mState |= MASK_BOUNCE_STATE(DebouncedStateChanged);
      }
    }
    return mState & MASK_BOUNCE_STATE(DebouncedStateChanged);
  }

  inline bool bouncePromtDetection() {
    bool readState = this->Input::read();
    // Clear Changed State Flag - will be reset if we confirm a button mState change.
    mState &= ~MASK_BOUNCE_STATE(DebouncedStateChanged);
    if (readState != (bool)(mState & MASK_BOUNCE_STATE(DebouncedState))) {
      // We have seen a change from the current button mState.
      if (millis() - mPrevTime >= mIntervalTime ) {
        // We have passed the time threshold, so a new change of mState is allowed.
        // set the DebouncedStateChanged flag and the new DebouncedState.
        // This will be prompt as long as there has been greater than interval_misllis ms since last change of input.
        // Otherwise debounced mState will not change again until bouncing is stable for the timeout period.
        mState ^= MASK_BOUNCE_STATE(DebouncedState);
        mState |= MASK_BOUNCE_STATE(DebouncedStateChanged);
      }
    }

    // If the readState is different from previous readState, reset the debounce timer - as input is still unstable
    // and we want to prevent new button mState changes until the previous one has remained stable for the timeout.
    if (readState != (bool)(mState & MASK_BOUNCE_STATE(DebouncedStateUnstable))) {
      // Update Unstable Bit to match readState
        mState ^= MASK_BOUNCE_STATE(DebouncedStateUnstable);
        mPrevTime = millis();
    }
    // return just the sate changed bit
    return mState & MASK_BOUNCE_STATE(DebouncedStateChanged);
  }

  inline bool bounceStable() {
    // Read the mState of the switch in a temporary variable.
    bool currentState = this->Input::read();
    mState &= ~MASK_BOUNCE_STATE(DebouncedStateChanged);

    // If the reading is different from last reading, reset the debounce counter
    if (currentState != (bool)(mState & MASK_BOUNCE_STATE(DebouncedStateUnstable))) {
        mPrevTime = millis();
        mState ^= MASK_BOUNCE_STATE(DebouncedStateUnstable);
    } else if (millis() - mPrevTime >= mIntervalTime) {
        // We have passed the threshold time, so the input is now stable
        // If it is different from last mState, set the DebouncedStateChanged flag
        if ((bool)(mState & MASK_BOUNCE_STATE(DebouncedState)) != currentState) {
          mPrevTime = millis();
          mState ^= MASK_BOUNCE_STATE(DebouncedState);
          mState |= MASK_BOUNCE_STATE(DebouncedStateChanged);
        }
    }

    return mState & MASK_BOUNCE_STATE(DebouncedStateChanged);
  }
};

END_IO_NAMESPACE
