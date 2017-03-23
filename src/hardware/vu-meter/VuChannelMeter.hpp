#include <muxer/IoDemuxer4051.hpp>

#include <Throttle.hpp>
#include "VuMeter.h"

class VuChannelMeter {
  private:
    io::Demuxer595& mDemux;
    uint8_t mChannel;
    uint16_t mFadeTime, mLevel, mTargetLevel, mPeak, mCurPeak;
    bool mClipHold, mPeakHold, mClip;
    throttle::Throttle mThrottle, mPeakHoldThrottle;

    inline bool isClipLevel(uint16_t value) {
      return mClipHold && value == PeakLed;
    }

  public:

    // demuxer has another demuxer daisy chained
    VuChannelMeter(io::Demuxer595& demuxer, uint8_t channel, bool clipHold = true,
        bool peakHold = false, uint16_t peakHoldTime = 2500, uint16_t fadeTime = 1200)
      : mDemux(demuxer)
      , mChannel(channel)
      , mFadeTime(fadeTime)
      , mLevel(0)
      , mTargetLevel(0)
      , mPeak(0)
      , mCurPeak(0)
      , mClipHold(clipHold)
      , mPeakHold(peakHold)
      , mClip(false) {
        mThrottle.setThreshold(DefaultVuMeterThreshold);
        mPeakHoldThrottle.setThreshold(peakHoldTime);
      }

    virtual ~VuChannelMeter() {}

    void setPeakHold(bool peakHold, uint16_t peakHoldTime) {
      mPeakHold = peakHold;
      mPeakHoldThrottle.setThreshold(peakHoldTime);
    }

    void setClipHold(bool clipHold) {
      mClipHold = clipHold;
    }

    void setFadeTime(uint16_t fadeTime) {
      mFadeTime = fadeTime;
    }

    void setChannel(uint8_t channel) {
      mChannel = channel;
      reset();
    }

    inline void set(uint16_t* levels) {
      setValue(levels[mChannel], false, false);
    }

    inline void setValue(int value, bool resetPeak = true, bool resetClip = true) {
      if (resetPeak) {
        mPeak = 0;
        mPeakHoldThrottle.invalidate();
      }

      if (resetClip) {
        mClip = false;
      }

      if (value >= VuMeterLen) {
        return;
      }

      // force update to not run in low level timeouts
      if (value > mTargetLevel) {
        mThrottle.setThreshold(DefaultVuMeterThreshold);
      }

      mTargetLevel = value;

      if (mClipHold && mTargetLevel == PeakLed) {
        mClip = true;
      }
    }

    inline void reset() {
      mClip = false;
      mPeak = 0;
      mTargetLevel = 0;
      mThrottle.invalidate();
      mPeakHoldThrottle.invalidate();
    }

    inline void update() {
      if (mThrottle.shouldUpdate()) {
        mThrottle.reset();

        uint16_t fadeTime = DefaultVuMeterThreshold;
        if (mTargetLevel < mLevel) {
          mLevel--;
          fadeTime = (mFadeTime / 3) / (mLevel + 1);
        } else if (mTargetLevel > mLevel) {
          mLevel++;
          fadeTime = (mFadeTime / 30) / (mLevel + 1);
        }

        if (mFadeTime > 0) {
          mThrottle.setThreshold(fadeTime);
        }

        mDemux.writeAll(VuMeterMapping[mLevel], VuMeterItemLen, false);

        if (mPeakHold) {
          if (mPeak < mLevel) {
            mPeak = mLevel;
            mPeakHoldThrottle.reset();
          } else {
            if (mCurPeak > mLevel && mCurPeak != mPeak && !isClipLevel(mCurPeak)) {
              mDemux.setPin(mCurPeak, LOW, false);
            }
            mCurPeak = mPeak;
            mDemux.setPin(mCurPeak, HIGH, false);
          }
        }

        if (mClipHold) {
          if (mClip) {
            mDemux.setPin(PeakLed, HIGH, false);
          } else if (!isClipLevel(mLevel)) { // reset clip
            mDemux.setPin(PeakLed, LOW, false);
          }
        }

        mDemux.update();
      }

      if (mPeakHold && mCurPeak != 0 && mPeakHoldThrottle.shouldUpdate()) {
        mPeakHoldThrottle.reset();
        if (mCurPeak > mPeak && !isClipLevel(mCurPeak)) {
          mDemux.setPin(mCurPeak, LOW, true);
        }
        mCurPeak = 0;
        mPeak = 0;
      }
    }
  };
