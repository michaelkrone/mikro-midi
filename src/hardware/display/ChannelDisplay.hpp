/*
   mikroMIDI main
*/

#include <MackieDefs.h>
#include "../lib/fonts/RobotoMedium10pt7b.h"
#include "../lib/fonts/RobotoMedium16pt7b.h"

#include "ChannelDisplay.h"

class ChannelDisplay {
  public:
    ChannelDisplay(io::DisplayHTDSDI96& display, uint8_t channel)
      : mDisplay(display)
      , mChannel(channel) {}

    virtual ~ChannelDisplay() {}

    inline void setupDisplay() {
      mDisplay.initialize();
      delay(30);
      mDisplay.setRotation(2);
      initDisplay();
    }

    // vpots
    inline void setVpot(const mackie::vPotRingSegmentDisplay vPotState) {
      int8_t start = -1;
      int8_t stop = -1;

      for (uint8_t i = 0; stop == -1 && i < mackie::VPotRingDisplayLength; i++) {
        if (vPotState[i] == 1) {
          if (start == -1) {
            start = i;
          }
        } else if (start != -1 && stop == -1){
          stop = i;
        }
      }

      if (start == -1 || stop == -1) {
        mVpotCache[0] = 0;
        mVpotCache[1] = 0;
        mVpotInvert = false;
        return;
      }

      //blockCnt == 1 means pan map, display a single square only
      uint16_t blockCnt = stop - start;
      uint8_t w = blockCnt == 1 ? VPOT_BAR_WIDTH : blockCnt * VPOT_SEGMENT_LENGTH;
      uint8_t x = start * VPOT_SEGMENT_LENGTH;

      if (blockCnt == 1) {
        w = VPOT_BAR_WIDTH;
        if (start != 0) {
          // center the bar to the middle of the value
          x += (VPOT_BAR_WIDTH / 2) - 1;
        }
      } else if (start == mackie::VPotRingCenter) {
        // if on first position align left, do not overlap
        x = DISPLAY_CENTER_X;
      } else {
        // shift by padding and border
        x += DISPLAY_PADDING + BORDER_WIDTH;
        if (stop == mackie::VPotRingCenter + 1) {
          // on last position align to the right border, do not overlap
          w = (DISPLAY_CENTER_X - x);
        }
      }

      mVpotInvert = /*blockCnt != 1 && */ x < DISPLAY_CENTER_X && x + w > DISPLAY_CENTER_X;
      mVpotCache[0] = x;
      mVpotCache[1] = w;
    }

    inline void setText(uint16_t* lcdState) {
      initDisplay();
      mDisplay.setFont(&Roboto_Medium16pt7b);
      mDisplay.setCursor(0, 23);

      uint8_t i = 0;
      const uint8_t* addr = mackie::LCD_INDEX_MAP[mChannel];

      // read upper row
      for (i = addr[0]; i < addr[1]; i++) {
          mDisplay.print((char) lcdState[i]);
      }

      // read lower row
      mDisplay.setFont(&Roboto_Medium10pt7b);
      mDisplay.setCursor(0, 44);
      for (i = addr[2]; i < addr[3]; i++) {
        mDisplay.print((char) lcdState[i]);
      }

      drawVpot();
    }

    inline void update() {
      mDisplay.update();
    }

    inline void drawSplash() {
      mDisplay.clear();
      mDisplay.setFont(&Roboto_Medium16pt7b);
      mDisplay.setCursor(20, 40);
      mDisplay.print("mikro");
      mDisplay.setCursor(58, SSD1306_HEIGHT - 3);
      mDisplay.setFont(&Roboto_Medium10pt7b);
      mDisplay.print("MIDI");
      mDisplay.update();
    }

  private:
    io::DisplayHTDSDI96& mDisplay;
    uint8_t mChannel;
    uint8_t mVpotCache[2];
    bool mVpotInvert = false;

    inline void initDisplay(bool clear = true) {
      if (clear) {
        mDisplay.clear();
      }
      mDisplay.setTextSize(1);
      mDisplay.setTextColor(WHITE, BLACK);
    }

    inline void drawScale(bool clear = false, bool drawAll = true, bool invertCenter = false) {
      if (clear) {
        mDisplay.fillRect(DISPLAY_PADDING, SSD1306_HEIGHT - SCALE_HEIGHT, SCALE_WIDTH, SCALE_HEIGHT, BLACK);
      }

      // outline
      if (drawAll) {
        mDisplay.drawRect(DISPLAY_PADDING, SSD1306_HEIGHT - SCALE_HEIGHT, SCALE_WIDTH, SCALE_HEIGHT, WHITE);
      }

      // center line
      mDisplay.drawLine(
        DISPLAY_CENTER_X, SSD1306_HEIGHT - (SCALE_HEIGHT - BORDER_WIDTH),
        DISPLAY_CENTER_X, SSD1306_HEIGHT - (BORDER_WIDTH * 2), invertCenter ? BLACK : WHITE);
    }

    inline void drawVpot() {
      // no width
      if (mVpotCache[1] <= 0) {
        return;
      }

      mDisplay.fillRect(
        mVpotCache[0], SSD1306_HEIGHT - SCALE_HEIGHT, mVpotCache[1], SCALE_HEIGHT, WHITE);
      drawScale(false, true, mVpotInvert);
    }
  };
