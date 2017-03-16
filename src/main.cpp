
/*
   mikroMIDI main
*/

// This optional setting causes Encoder to use more optimized code,
// It must be defined before Encoder.h is included.
// #define ENCODER_OPTIMIZE_INTERRUPTS
// #include <Encoder.h>

#include <Throttle.hpp>
#include <IoOutput.hpp>
#include <IoTouchInput.hpp>
#include <IoResponsiveAnalogInput.hpp>
#include <ic/IoIcL293.hpp>
#include <fader/IoMotorFader.hpp>

io::ResponsiveAnalogInput faderLevel(14, true);
io::TouchInput faderTouch(15);
// io::Output enable(22);
// io::Output motorA(20, true);
// io::Output motorB(21, true);
// io::IcL293 hBridge(&enable, &motorA, &motorB);
// io::MotorFader fader(faderLevel, &faderTouch, hBridge, 0, 1, 4096);

throttle::Throttle faderThrottle(1000);

int tlen = 10;
int target[] = {1022, 40, 100, 500, 499, 550, 1022, 800, 300, 900};

void setup() {
  analogWriteResolution(12);
  analogWriteFrequency(20, 8789.062);
  analogWriteFrequency(21, 8789.062);
  // hBridge.stopAll();

  Serial.begin(9600);
  delay(1225);
  pinMode(13, OUTPUT);
  digitalWrite(13, 1);
  delay(1225);
  digitalWrite(13, 0);
  Serial.println("starting .....: ");
  // fader.set(500);
}

// int pos = 0;
int pos = 0;
void loop() {
  // fader.update();
  if (faderThrottle.shouldUpdate()) {
    faderTouch.update();
    faderLevel.update();
    faderThrottle.reset();
    Serial.print("touch level: ");Serial.println(faderTouch.read());
    Serial.print("touched    : ");Serial.println(faderTouch.isTouched());
    Serial.print("level      : ");Serial.println(faderLevel.read());
  }
  //   if (pos < tlen)
  //     fader.set(target[pos++]);
  //   else
  //     pos = 0;
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
