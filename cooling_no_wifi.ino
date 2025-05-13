const int LIVING = 4;
const int BED1 = 27;
const int BED2 = 25;
const int SPEED1 = 19;
const int SPEED2 = 21;
const int SPEED3 = 22;
const int ZoneValve = 23;



void initPins() {
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(LIVING, INPUT_PULLDOWN);
  pinMode(BED1, INPUT_PULLDOWN);
  pinMode(BED2, INPUT_PULLDOWN);
  pinMode(SPEED1, OUTPUT);
  pinMode(SPEED2, OUTPUT);
  pinMode(SPEED3, OUTPUT);
  pinMode(ZoneValve, OUTPUT);
}

int getSpeed() {
  int speed = 0;
  switch (digitalRead(BED1)) {
    case HIGH:
      speed += 1;
      break;
    case LOW:
      break;
  }
  switch (digitalRead(BED2)) {
    case HIGH:
      speed += 1;
      break;
    case LOW:
      break;
  }
  switch (digitalRead(LIVING)) {
    case HIGH:
      speed += 2;
      break;
    case LOW:
      break;
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
