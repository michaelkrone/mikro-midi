#include <muxer/IoDemuxer595.hpp>

#include <Throttle.hpp>
#include "VuMeter.h"

class VuChannelMeter {
  private:
    io::Demuxer595& mDemux;
    uint8_t mChannel;
    uint16_t mOffset, mLevel, mTargetLevel, mPeak, mCurPeak;
    bool mClipHold, mPeakHold, mClip;
    throttle::Throttle mThrottle, mPeakHoldThrottle;
    uint16_t mFadeTime, mUpFade, mDownFade;

    inline bool isClipLevel(uint16_t value) {
      return mClipHold && mClip && value == PeakLed;
    }

  public:
    // demuxer has at least one other demuxer daisy chained
    VuChannelMeter(io::Demuxer595& demuxer, uint8_t channel, bool clipHold = true,
        bool peakHold = false, uint16_t peakHoldTime = 3600, uint16_t fadeTime = 1200)
      : mDemux(demuxer)
      , mChannel(channel)
      , mOffset(channel * VuMeterItemLen)
      , mLevel(0)
      , mTargetLevel(0)
      , mPeak(0)
      , mCurPeak(0)
      , mClip(false) {
        setFadeTime(fadeTime);
        setPeakHold(peakHold, peakHoldTime);
        setClipHold(clipHold);
        mThrottle.setThreshold(DefaultVuMeterThreshold);
        mDemux.setAutoWrite(false);
        mDemux.disable();
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
      mUpFade = mFadeTime / (VuMeterItemLen * 5);
      mDownFade = mFadeTime / (VuMeterItemLen / 5);
    }

    void setChannel(uint8_t channel) {
      mChannel = channel;
      reset();
    }

    inline void setValues(uint16_t* levels) {
      setValue(levels[mChannel]);
    }

    inline void setValue(int value) {
      if (value >= VuMeterLen) {
        return;
      }

      // force update to not run in low level timeouts
      if (value > mTargetLevel) {
        mThrottle.setThreshold(DefaultVuMeterThreshold);
      }

      mTargetLevel = value;

      if (mClipHold && !mClip && mTargetLevel == PeakLed) {
        mClip = true;
      }
    }

    inline void reset() {
      mClip = false;
      mTargetLevel = 0;
      mPeak = 0;
      mPeakHoldThrottle.invalidate();
      mThrottle.invalidate();
    }

    inline void update() {
      if (mThrottle.shouldUpdate()) {
        mThrottle.reset();

        uint16_t fadeTime = DefaultVuMeterThreshold;
        if (mTargetLevel < mLevel) {
          mLevel--;
          fadeTime = mDownFade / (mLevel + 1);
        } else if (mTargetLevel > mLevel) {
          mLevel++;
          fadeTime = mUpFade / (mLevel + 1);
        }

        if (mFadeTime > 0) {
          mThrottle.setThreshold(fadeTime);
        }

        mDemux.enable();
        // we need to add the offset for more than 1 channel daisy chained
        // mDemux.writeAll(VuMeterMapping[mLevel], VuMeterItemLen, false);
        for (uint8_t i = 0; i < VuMeterItemLen; i++) {
          mDemux.setPin(mOffset + i, VuMeterMapping[mLevel][i]);
        }

        if (mPeakHold && mLevel > 0) {
          if (mPeak < mLevel) {
            mPeak = mLevel;
            mPeakHoldThrottle.reset();
          } else {
            if (mCurPeak > mLevel && mCurPeak != mPeak && !isClipLevel(mCurPeak)) {
              mDemux.setPin(mOffset + mCurPeak, LOW);
            }

            mCurPeak = mPeak;
            mDemux.setPin(mOffset + mCurPeak, HIGH);
          }
        }

        if (mClipHold) {
          if (mClip) {
            mDemux.setPin(mOffset + PeakLed, HIGH);
          } else if (!isClipLevel(mLevel)) { // reset clip
            mDemux.setPin(mOffset + PeakLed, LOW);
          }
        }

        mDemux.update();
      }

      if (mPeakHold && mCurPeak != 0 && mPeakHoldThrottle.shouldUpdate()) {
        mPeakHoldThrottle.reset();

        if (mCurPeak > mPeak && !isClipLevel(mCurPeak)) {
          mDemux.writePin(mOffset + mCurPeak, LOW);
        }

        mCurPeak = 0;
        mPeak = 0;
      }
    }
  };
