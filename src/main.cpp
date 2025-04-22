#include "definitions.h"

void setup() {
  pinMode(LED, OUTPUT);
  
  pinMode(trigP, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echoP, INPUT);   // Sets the echoPin as an Input
  
  pinMode(parkFree, OUTPUT);
  pinMode(parkOcc, OUTPUT);
  
  if(serial) Serial.begin(115200);      
  delay(10);
  if(serial) Serial.println();
  if(serial) Serial.println("Initiating Device...");
  delay(10);

  EEPROM.begin(EEPROM_SIZE);
  
  WiFi_httpStuff(); //handles everything wrt wifi and http server

  client.setServer(SERVER, SERVERPORT);
  client.setCallback(callback);
  getDistance();
  registerDevice();
}

//
void loop() {
  httpServer.handleClient();

  getDistance();
  checkConnection();
  connectSubscribe();
  publishFeeds();

  client.loop();  
  yield();
}