#ifndef webupdater_h
#define webupdater_h
#include "Arduino.h"

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include "defines.h"

extern WiFiClientSecure wificlientsecure;
// create MQTT object
extern WiFiClient wificlient; //This is the client used for MQTT connection
extern HTTPClient rpihttp; //HTTP client for RPI server
extern PubSubClient client;
// extern const char* host;
extern bool serial, wifiConnected;
extern bool configFreshlySaved;
extern char SERVER[16];

// extern ESP8266WiFiMulti wifiMulti;
extern ESP8266WebServer httpServer; //Choose any number you want, just add this after your ip adrs
extern ESP8266HTTPUpdateServer httpUpdater; //http://<local-ip>:<any-number>/ as server address

extern const char* loginIndex;
extern const char* serverIndex;

// Your GitHub-hosted firmware URL
extern const char* firmwareURL;

// Parameters from intial setup
extern char rpiServer[RPI_IP_SIZE];
extern char deviceName[DEVICE_NAME_SIZE];
extern char mqttUsername[MQTT_USERNAME_SIZE];
extern char mqttKey[MQTT_KEY_SIZE];
extern char deviceUUID[36]; // UUID for the device
extern unsigned int parkSpaceVehicleDistance, minVehDistance, maxVehDistance; //in cm

void performOTAUpdate();
void WiFi_httpStuff();
bool registerDevice();
void saveConfig();
void startHTTPServer();
void setupHTTPRoutes();

#endif