#include <ESP32Servo.h>

Servo servob1;
Servo servob2;
Servo servol1;
Servo servol2;

const int LIVING = 4;
const int BED1 = 27;
const int BED2 = 25;
const int SPEED1 = 19;
const int SPEED2 = 21;
const int SPEED3 = 22;
const int ZoneValve = 23;
const int SERVOB1 = 14;
const int SERVOB2 = 12;
const int SERVOL1 = 13;
const int SERVOL2 = 15;



void initPins() {
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(LIVING, INPUT_PULLDOWN);
  pinMode(BED1, INPUT_PULLDOWN);
  pinMode(BED2, INPUT_PULLDOWN);
  pinMode(SPEED1, OUTPUT);
  pinMode(SPEED2, OUTPUT);
  pinMode(SPEED3, OUTPUT);
  pinMode(ZoneValve, OUTPUT);

  servob1.attach(SERVOB1);
  servob2.attach(SERVOB2);
  servol1.attach(SERVOL1);
  servol2.attach(SERVOL2);
}

int getSpeed() {
  int speed = 0;
  switch (digitalRead(BED1)) {
    case HIGH:
      speed += 1;
      servob1.write(100);
      break;
    case LOW:
      servob1.write(0);
  }
  switch (digitalRead(BED2)) {
    case HIGH:
      speed += 1;
      servob2.write(100);
      break;
    case LOW:
      servob2.write(0);
  }
  switch (digitalRead(LIVING)) {
    case HIGH:
      speed += 2;
      servol1.write(100);
      servol2.write(100);
      break;
    case LOW:
      servol1.write(0);
      servol2.write(0);
  }
  if (speed > 3) {
    speed = 3;
  }
  //values["speed"] = speed;
  return speed;
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  initPins();
  Serial.println("All Active");
}

void loop() {
  int speed = getSpeed();
  switch (speed) {
    case 1:
      digitalWrite(ZoneValve, HIGH);
      digitalWrite(SPEED1, HIGH);
      digitalWrite(SPEED2, LOW);
      digitalWrite(SPEED3, LOW);
      break;
    case 2:
      digitalWrite(ZoneValve, HIGH);
      digitalWrite(SPEED1, LOW);
      digitalWrite(SPEED2, HIGH);
      digitalWrite(SPEED3, LOW);
      break;
    case 3:
      digitalWrite(ZoneValve, HIGH);
      digitalWrite(SPEED1, LOW);
      digitalWrite(SPEED2, LOW);
      digitalWrite(SPEED3, HIGH);
      break;
    default:
      digitalWrite(ZoneValve, LOW);
      digitalWrite(SPEED1, LOW);
      digitalWrite(SPEED2, LOW);
      digitalWrite(SPEED3, LOW);
  }
  delay(500);
}
