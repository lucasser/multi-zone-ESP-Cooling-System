// Import required libraries
#include <AsyncElegantOTA.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>
#include <ESP32Servo.h>

Servo servob1;
Servo servob2;
Servo servol1;
Servo servol2;

JSONVar values;

String ssids;
String passwords;

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

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

AsyncEventSource events("/events");

using namespace std;

void initSPIFFS() {
  if (!SPIFFS.begin()) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
  File file = SPIFFS.open("/wifi.txt");
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  vector<String> v;
  while (file.available()) {
    v.push_back(file.readStringUntil('\n'));
  }
  file.close();

  for (String s : v) {

  }
  ssids = v[0];
  passwords = v[1];
}

void initWiFi() {
  const char* ssid = ssids.c_str();
  const char* password = passwords.c_str();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);
    digitalWrite(LED_BUILTIN, LOW);
    delay(50);
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.macAddress());
}

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
      values["bed1"] = "on";
      servob1.write(100);
      break;
    case LOW:
      values["bed1"] = "off";
      servob1.write(0);
  }
  switch (digitalRead(BED2)) {
    case HIGH:
      speed += 1;
      values["bed2"] = "on";
      servob2.write(100);
      break;
    case LOW:
      values["bed2"] = "off";
      servob2.write(0);
  }
  switch (digitalRead(LIVING)) {
    case HIGH:
      speed += 2;
      values["living"] = "on";
      servol1.write(100);
      servol2.write(100);
      break;
    case LOW:
      values["living"] = "off";
      servol1.write(0);
      servol2.write(0);
  }
  if (speed > 3) {
    speed = 3;
  }
  values["speed"] = speed;
  return speed;
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  initPins();
  initSPIFFS();
  //initWiFi();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
  server.serveStatic("/", SPIFFS, "/");

  server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request){
    getSpeed();
    String jsonString = JSON.stringify(values);
    request->send(200, "application/json", jsonString);
  });

  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);
  
  AsyncElegantOTA.begin(&server);
  server.begin();
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
  String jsonString = JSON.stringify(values);
  events.send("ping",NULL,millis());
  events.send(jsonString.c_str(),"new_readings", millis());
  delay(500);
}
