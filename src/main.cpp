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
  if (serial) Serial.print("Vehicle distance from config: ");
  if (serial) Serial.println(parkSpaceVehicleDistance);

  Serial.println("Loaded Configuration:");
  Serial.print("RPI IP Address: "); Serial.println(rpiServer);
  Serial.print("Device Name: "); Serial.println(deviceName);
  Serial.print("MQTT Username: "); Serial.println(mqttUsername);
  Serial.print("MQTT Key: "); Serial.println(mqttKey);
  // getDistance();
  if (!isDeviceRegistered()) registerDevice();
  // else updateDeviceRegistry();
  String str = getDeviceUUID();
  str.toCharArray(deviceUUID, UUID_LENGTH+1); // Store UUID in global variable
  if (serial) Serial.print("Device UUID: ");
  if (serial) Serial.println(deviceUUID);
  if (String(deviceUUID) == String(defaultUUID)) {
    if (serial) Serial.println("Device UUID is default. Please register the device.");
    if (serial) Serial.println("Complete the device setup first by visiting:.");
    if (serial) {
      Serial.print("http://");
      Serial.print(WiFi.localIP());
      Serial.print("/devicesetup/");
      Serial.println(defaultUUID);
    }
  } else {
    if (serial) Serial.println("Device registered successfully.");
  }

  setupHTTPRoutes();
  startHTTPServer(); // Start the HTTP server after WiFi connection

  initiateUltrasonicSensor(); // Initialize ultrasonic sensor
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