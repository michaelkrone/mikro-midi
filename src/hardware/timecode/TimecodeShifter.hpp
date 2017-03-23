
/*
   mikroMIDI main
*/

#include <MackieDefs.h>

#include "TimecodeShifter.h"

class TimecodeShifter {
  private:
    io::Demuxer595& mDemuxer;
    uint8_t mStartIndex;
    uint8_t mDigits;
    bool mCommonAnode;

  /**
  Demuxer will have at least 2 shift registers chained: 1 decimal point and 1 enable control
  */

  public:
    TimecodeShifter(io::Demuxer595& demuxer, uint8_t digitStartIndex = 0, bool commonAnode = true)
      : mDemuxer(demuxer)
      , mStartIndex(digitStartIndex)
      , mCommonAnode(commonAnode) {
        mDemuxer.setAutoWrite(false);
        // write 2 digits at once
        mDigits = (mDemuxer.getChannelLength() - 2) * 2;
      }

    virtual ~TimecodeShifter() {}

    inline void setTimecode(uint16_t* timecode) {
      uint8_t pin = 0;
      uint8_t code = 0;

      for (uint8_t i = 0; i < mDigits; i++) {
        code = timecode[mStartIndex + i];

        // write binary digit
        for (uint8_t j = 0; j < LED_CHAR_LEN; j++) {
          mDemuxer.writePin(pin++, CHAR_MAP[code][j]);
        }

        // activate dp if requested
        if (DP_START_INDEX) {
          mDemuxer.writePin(
            DP_START_INDEX + i, DISPLAY_TIMECODE_DOT(code) ? !mCommonAnode : mCommonAnode);
        }

        // write state all 2 digits
        if ((i + 1) % 2 == 0) {
          enable(i - 1);
          disable(i - 1);
          pin = 0;
        }
      }
    }

    inline void update() {
      mDemuxer.writeAll();
    }

    inline void enable(uint8_t pin) {
      mDemuxer.writePin(ENABLE_START_INDEX + pin, 0, true);
      // update();
    }

    inline void disable(uint8_t pin) {
      mDemuxer.writePin(ENABLE_START_INDEX + pin, 1, true);
      // update();
    }
  };
