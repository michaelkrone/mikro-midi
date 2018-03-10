
/*
   mikroMIDI main
*/

#define ANALOG_MAX 4096

// #include <MackieProtocol.hpp>
// mackie::MackieProtocol mp(mackie::OperationMode_MackieControl);

#include <Throttle.hpp>
#include <output/IoOutput.hpp>
#include <ic/IoIcMax72.hpp>

throttle::Throttle t1(500);
throttle::Throttle t2(3000);

io::IcMax72 driver(10);

void setup() {
  // analogWriteResolution(12);
  // analogWriteFrequency(23, 8789.062);
  // analogWriteFrequency(22, 8789.062);

  Serial.begin(9600);
  delay(225);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  // fader.setValue(0);
  delay(500);
  digitalWrite(13, LOW);
  Serial.println("starting .....: ");
  // mp.begin();
}

void loop() {
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  delay(500);
  // read data
  // mp.update();

  if (t1.shouldUpdate()) {
    t1.reset();
    Serial.println("update 1");
  }

  if (t2.shouldUpdate()) {
    t2.reset();
    Serial.println("update 2");
  }
}
