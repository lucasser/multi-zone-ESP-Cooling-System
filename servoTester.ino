#include <ESP32Servo.h>

Servo servo1;
Servo servo2;

const int SWITCH = 4;
const int SERVO1 = 13;
const int SERVO2 = 15;

void initPins() {
  pinMode(SWITCH, INPUT_PULLDOWN);

  servol1.attach(SERVO1);
  servol2.attach(SERVO2);
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  initPins();
}

void loop() {
  switch (digitalRead(SWITCH)) {
    case HIGH:
      Serial.println("high");
      servob1.write(100);
      break;
    case LOW:
      Serial.println("low");
      servob1.write(0);
  }
  delay(500);
}
