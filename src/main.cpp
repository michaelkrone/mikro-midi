
/*
   mikroMIDI main
*/

#include <MackieProtocol.hpp>
mackie::MackieProtocol mp(mackie::OperationMode_MackieControl);

#include <Throttle.hpp>
#include <output/IoOutput.hpp>
// #include <input/IoTouchInput.hpp>
// #include <input/IoResponsiveAnalogInput.hpp>
// #include <ic/IoIcL293.hpp>
// #include <pid/IoPidController.hpp>

// #include <muxer/IoMuxer4051.hpp>
#include <muxer/IoDemuxer595.hpp>

#include <ic/IoIc595.hpp>
#include <muxer/IoDemuxer595.hpp>
#include "hardware/vu-meter/VuChannelMeter.hpp"

io::Ic595 ic595(31, 30, 29, 2);
io::Demuxer595 demuxer(ic595, 0, false);

VuChannelMeter meter(demuxer, 0);

// io::Ic4051 ic4051(14, 15, 16, 22, 8, 100);
// io::Demuxer4051 demuxer(ic4051, true);

// io::ResponsiveAnalogInput faderLevel(14, true);
// io::TouchInput faderTouch(15);
// io::Output enable(22);
// io::Output motorA(20, true);
// io::Output motorB(21, true);
// io::IcL293 hBridge(&enable, &motorA, &motorB);
// io::PidController fader(faderLevel, hBridge, 0, 1, 4096);

throttle::Throttle faderThrottle(5);

int tlen = 10;
int target[] = {12, 11, 10, 9, 8, 5, 6, 7, 8, 9};

void setup() {
  // analogWriteResolution(12);
  // analogWriteFrequency(20, 8789.062);
  // analogWriteFrequency(21, 8789.062);
  // hBridge.stopAll();

  // Serial.begin(9600);
  delay(225);
  pinMode(13, OUTPUT);
  digitalWrite(13, 1);
  delay(525);
  digitalWrite(13, 0);
  mp.begin();
  // Serial.println("starting .....: ");
}

// int pos = 0;
int pos = 0;
void loop() {
  uint8_t* changed = mp.update();
  meter.update();

  if (changed[mackie::States::VuMeters]) {
    uint16_t* meterValues = mp.getVuMeters();
    meter.set(meterValues);
  }

  if (changed[mackie::States::Command]) {
    uint16_t* cmds = mp.getCommands();
    if (cmds[mackie::Commands::Transport_Stop]) {
      meter.reset();
    }
  }
  // fader.update();
  // if (faderThrottle.shouldUpdate()) {
  //   faderThrottle.reset();
  //   meter.update();
    // faderTouch.update();
    // faderLevel.update();
    // Serial.print("touch level: ");Serial.println(faderTouch.read());
    // Serial.print("touched    : ");Serial.println(faderTouch.isTouched());
    // Serial.print("level      : ");Serial.println(faderLevel.read());
    // if (pos < tlen)
    //   meter.set(target[pos++]);
    // else
    //   pos = 0;
      // Serial.print("setting target ");
      // Serial.print(target[pos]);
  // }
  //   // Serial.print("target: ");Serial.println(target[pos]);
  //   // faderLevel.update();
  //   // Serial.print("plant: ");Serial.println(fader.mPlant);
  //   // hBridge.enable(0);
  //   // Serial.print("down: 2500");
  //   // hBridge.down(0, 2800);
  //   // delay(1000);
  //   // hBridge.stop(0);
  //   // hBridge.enable(0);
  //   // Serial.print("up: 2000");
  //   // hBridge.up(0, 2700);
  //   // delay(2000);
  //   // hBridge.stop(0);
  // //   // fader.update();
  //
  // //
  // //   // digitalWrite(13, 1);
  // //   for (int i = 0; i < tlen; i++) {
  // //     Serial.println("________________");Serial.println(target[i]);
  // //     fader.moveTo(target[i]);
  // //     Serial.print("level: ");Serial.println(faderLevel.read());
  // //     Serial.println("________________");
  // //     delay(1000);
  // //   }
  // //     // fader.set(target[i]);
  // //     // hBridge.disable(0);
  // //   // digitalWrite(13, 0);
  // }
}
