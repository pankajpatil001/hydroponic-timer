#ifndef config_h
#define config_h
#include "Arduino.h"

// #include <ESP8266WiFi.h>
// #include <ESP8266WebServer.h>
#include <EEPROM.h>
// #include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include "defines.h"

// extern ESP8266WebServer httpServer; 
extern bool configFreshlySaved;
extern char rpiServer[RPI_IP_SIZE];
extern char deviceName[DEVICE_NAME_SIZE];
extern char mqttUsername[MQTT_USERNAME_SIZE];
extern char mqttKey[MQTT_KEY_SIZE];
extern char onTimeFeed[FEED_SIZE], offTimeFeed[FEED_SIZE], msgsFeed[FEED_SIZE];

// extern unsigned int parkSpaceVehicleDistance, minVehDistance, maxVehDistance; //in cm
extern unsigned int onTime, offTime, testTime; //in seconds

// EEPROM helper functions
void readFromEEPROM(int startAddr, char* dest, int size);
void writeToEEPROM(int startAddr, const char* src, int size);

// Config functions
void loadConfig();
void saveConfig();

#endif