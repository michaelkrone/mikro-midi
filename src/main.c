
/*
   mikroMIDI main
*/

// This optional setting causes Encoder to use more optimized code,
// It must be defined before Encoder.h is included.
// #define ENCODER_OPTIMIZE_INTERRUPTS
// #include <Encoder.h>

#include <Throttle.hpp>
#include <IoOutput.hpp>
// #include <IoBouncedInput.hpp>
// #include <rotary/RotaryEncoder.hpp>
// #include <muxer/IoMuxer4051.hpp>
#include <muxer/IoDemuxer595.hpp>
static const uint8_t IC_595_1_SHCP = 28;
static const uint8_t IC_595_1_STCP = 27;
static const uint8_t IC_595_1_DS = 26;
#include <MackieProtocol.hpp>
mackie::MackieProtocol mp(mackie::OperationMode_MackieControl);

//
#include <display/IoDisplayHTDSDI96.hpp>
#include "hardware/display/ChannelDisplay.hpp"

#include "hardware/timecode/TimecodeShifter.hpp"
//
// #include "Hardware.h"
//
// #define SHCP 27
// #define STCP 28
// #define DS 26
// static const uint8_t S0 = 31;
// static const uint8_t S1 = 32;
// static const uint8_t S2 = 33;
// static const uint8_t MUX = 14;
//
#define SDA_PIN 30
#define SCL_PIN 29
//
// long positionLeft  = -999;
// long positionRight = -999;ic595
//
// io::Ic4051 ic4051(S0, S1, S2, MUX, 5, 100);
// io::Ic4051 ic4051_2(S0, S1, S2, MUX + 1, 2, 100);
// io::Muxer4051 muxer(ic4051);
// io::Muxer4051 muxer2(ic4051_2);
//
io::Ic595 ic595(IC_595_1_SHCP, IC_595_1_STCP, IC_595_1_DS, 3);
io::Demuxer595 demuxer(ic595, false, 0, false);
TimecodeShifter mtc(demuxer);

io::DisplayHTDSDI96 ssdDisplay(SDA_PIN, SCL_PIN);
ChannelDisplay display1(ssdDisplay, 0);

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  delay(1225);
  digitalWrite(13, LOW);
  // Serial.begin(9600);
  mp.begin();
  display1.setupDisplay();
  display1.drawSplash();
}

throttle::Throttle mtcTimer(30);
throttle::Throttle displayTimer(35, true);
throttle::Throttle vPotTimer(35, true);

void loop() {
  uint8_t* changed = mp.update();

  if (changed[mackie::States::Timecode] && mtcTimer.shouldUpdate()) {
    mtc.setTimecode(mp.getTimecode());
    mtc.update();
    mtcTimer.reset();
  }

  if (changed[mackie::States::VPotRings]) {
    display1.setVpot(mackie::VPotRingDisplayMap[mp.getVPotRings()[0]]);
    displayTimer.invalidate();
  }

  if (changed[mackie::States::Lcd]) {
    display1.setText(mp.getLcd());
    displayTimer.invalidate();
  }

  if (displayTimer.shouldUpdate()) {
    display1.update();
    displayTimer.reset();
  }

  // int newLeft = knob.read();
  // if (knob.cw()) {
  //   Serial.print("Right = ");
  //   Serial.println(newLeft);
  //   positionLeft = newLeft;
  // }
  //
  // if (knob.ccw()) {
  //   Serial.print("Left = ");
  //   Serial.println(newLeft);
  //   positionLeft = newLeft;
  // }
  //
  // newLeft = knob2.read();
  // if (knob2.cw()) {
  //   Serial.print("2 Right = ");
  //   Serial.println(newLeft);
  //   positionLeft = newLeft;
  // }
  //
  // if (knob2.ccw()) {
  //   Serial.print("2 Left = ");
  //   Serial.println(newLeft);
  //   positionLeft = newLeft;
  // }
  //
  // btnKnop.update();
  // if (btnKnop.up()) {
  //   // mp.sendCommandOn(mackie::Commands::Transport_Play);
  //   Serial.println("BTN!");
  //   ledTest();
  // }

  // if (t.shouldUpdate()) {
  //   ledTest();
  //   t.reset();
  // }

  // uint8_t* changed = mp.update();
  //
  // if (changed[mackie::States::VPotRings] == mackie::State::Changed) {
  //   display1.setVpot(mackie::VPotRingDisplayMap[mp.getVPotRings()[0]]);
  //   shouldUpdateDisplay = true;
  // }
  //
  // if (changed[mackie::States::Lcd] == mackie::State::Changed) {
  //   display1.setName(mp.getLcd());
  //   shouldUpdateDisplay = true;
  // }
  //
  // if (shouldUpdateDisplay && millis() - lastDisplayUpdate >= 50) {
  //   display1.update();
  //   lastDisplayUpdate = millis();
  //   shouldUpdateDisplay = false;
  // }
  //
  // btnPlay.update();
  // btnStop.update();
  // //
  // if (btnPlay.up()) {
  //   // mp.sendCommandOn(mackie::Commands::Transport_Play);
  //   ledTest();
  // } else if (btnPlay.down()) {
  //   // mp.sendCommandOff(mackie::Commands::Transport_Play);
  // }
  //
  // if (btnStop.up()) {
  //   ledTest();
  //   // mp.sendCommandOn(mackie::Commands::Transport_Stop);
  // } else if (btnStop.down()) {
  //   // mp.sendCommandOff(mackie::Commands::Transport_Stop);
  // }
}
