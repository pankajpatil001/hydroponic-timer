#ifndef registerDevice_h
#define registerDevice_h
#include "Arduino.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include "defines.h"


extern WiFiClient wificlient; //This is the client used for WiFi / HTTP connection
extern bool serial;
extern char SERVER[64];
bool registerDevice();

#endif


