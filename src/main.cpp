
/*
   mikroMIDI main
*/

#define ANALOG_MAX 4096

#include <MackieProtocol.hpp>
mackie::MackieProtocol mp(mackie::OperationMode_MackieControl);

#include <Throttle.hpp>
#include <output/IoOutput.hpp>
// #include <input/IoTouchInput.hpp>
// #include <input/IoResponsiveAnalogInput.hpp>
// #include <ic/IoIcL293.hpp>
#include <pid/IoPidController.hpp>

// #include <muxer/IoMuxer4051.hpp>
#include <ic/IoIcL293.hpp>
#include <motor/IoMotorL9110.hpp>

#include <ic/IoIc595.hpp>
#include <muxer/IoDemuxer595.hpp>
#include "hardware/vu-meter/ChannelVuMeter.hpp"

// io::Ic595 ic595(31, 30, 29, 2);
// io::Demuxer595 demuxer(ic595, 0, false);
//
// VuChannelMeter meter(demuxer, 0);

// io::Ic4051 ic4051(14, 15, 16, 22, 8, 100);
// io::Demuxer4051 demuxer(ic4051, true);

// io::TouchInput faderTouch(15);
io::Output speed(23, true);
io::Output direction(22, true);
io::MotorL9110 driver(speed, direction);
// io::IoMotorL293 driver(icL293);
io::ResponsiveAnalogInput faderLevel(14, true);
io::PidController fader(faderLevel, driver, 2);

throttle::Throttle faderThrottle(5);

int tlen = 10;
int target[] = {12, 11, 10, 9, 8, 5, 6, 7, 8, 9};

void setup() {
  analogWriteResolution(12);
  analogWriteFrequency(23, 8789.062);
  analogWriteFrequency(22, 8789.062);

  // Serial.begin(9600);
  // Serial.println("starting .....: ");
  delay(225);

  fader.setValue(0);
  mp.begin();
}

uint16_t* cmds;
uint16_t* meterValues;
uint16_t* faderValues;

void loop() {
  // read data
  mp.update();
  //
  // if (mp.states[mackie::States::VuMeters]) {
  //   meterValues = mp.getVuMeters();
  //   meter.setValue(meterValues[0]);
  // }
  //
  if (mp.states[mackie::States::Faders]) {
    faderValues = mp.getFaders();
    fader.setValue(faderValues[0]);
  }
  //
  // if (mp.states[mackie::States::Command]) {
  //   cmds = mp.getCommands();
  //   if (cmds[mackie::Commands::Transport_Stop]) {
  //     meter.reset();
  //   }
  // }

  // write updated data

  fader.update();
  // Serial.println(faderLevel.read());
  // delay(1000);
  // meter.update();
}
