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

  loadConfig(); // Load saved config at boot

  Serial.println("Loaded Configuration:");
  Serial.print("RPI IP Address: "); Serial.println(rpiServer);
  Serial.print("Device Name: "); Serial.println(deviceName);
  Serial.print("MQTT Username: "); Serial.println(mqttUsername);
  Serial.print("MQTT Key: "); Serial.println(mqttKey);
  // getDistance();
  if (!isDeviceRegistered()) registerDevice();
  String str = getDeviceUUID();
  str.toCharArray(deviceUUID, UUID_LENGTH+1); // Store UUID in global variable
  if (serial) Serial.print("Device UUID: ");
  if (serial) Serial.println(deviceUUID);
  if (serial) Serial.println("Device registered successfully.");

  setupHTTPRoutes();
  startHTTPServer(); // Start the HTTP server after WiFi connection
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