#ifndef registerDevice_h
#define registerDevice_h
#include "Arduino.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include "defines.h"

extern HTTPClient rpihttp;
extern WiFiClient wificlient; //This is the client used for WiFi / HTTP connection
extern bool serial;
extern char SERVER[16];
extern char rpiServer[RPI_IP_SIZE];
extern char deviceName[DEVICE_NAME_SIZE];
extern char deviceUUID[36]; // UUID for the device
extern String defaultUUID; // Default UUID for the device
bool registerDevice();
bool isDeviceRegistered();
bool updateDeviceRegistry();
String getDeviceUUID();

#endif


