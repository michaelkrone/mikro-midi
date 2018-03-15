
/*
   mikroMIDI main
*/

#define ANALOG_MAX 4096

// #include <MackieProtocol.hpp>
// mackie::MackieProtocol mp(mackie::OperationMode_MackieControl);

#include <Throttle.hpp>
#include <output/IoOutput.hpp>
#include <ic/IoIc595.hpp>
#include <muxer/IoDemuxer595.hpp>

throttle::Throttle t1(1754);
throttle::Throttle t2(7434);
io::Output enable(32);
io::Ic595 ic595(30, 31, 29, 1, &enable);
io::Demuxer595 demuxer(ic595, false);

void setup() {
  // analogWriteResolution(12);
  // analogWriteFrequency(23, 8789.062);
  // analogWriteFrequency(22, 8789.062);

  Serial.begin(9600);

  delay(225);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);

  Serial.println("starting .....: ");
  demuxer.enable();
  // mp.begin();
}

void loop() {
  // digitalWrite(13, HIGH);
  // delay(500);
  // digitalWrite(13, LOW);
  // delay(500);
  // read data
  // mp.update();

  if (t1.shouldUpdate()) {
    t1.reset();
    Serial.println("update 1");
    demuxer.enable();
    demuxer.setPin(0, 1);
    demuxer.setPin(1, 1);
    demuxer.setPin(2, 1);
    demuxer.setPin(3, 0);
    demuxer.setPin(4, 1);
    demuxer.setPin(5, 0);
    demuxer.setPin(6, 1);
    demuxer.setPin(7, 1);
    demuxer.update();

  }

  if (t2.shouldUpdate()) {
    t2.reset();
    Serial.println("update 2");
    demuxer.setPin(0, 0);
    demuxer.setPin(1, 0);
    demuxer.setPin(2, 0);
    demuxer.setPin(3, 1);
    demuxer.setPin(4, 0);
    demuxer.setPin(5, 1);
    demuxer.setPin(6, 0);
    demuxer.setPin(7, 0);
    demuxer.disable();
    demuxer.update();
  }

}
