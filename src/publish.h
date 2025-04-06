#ifndef publish_h
#define publish_h
#include "Arduino.h"
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include "defines.h"

extern unsigned long tkeepRssi, tkeepUSpub;
extern unsigned int rssiTime, uspubTime;
extern bool firstTime, connection;
extern PubSubClient client;
extern char valueStr[100];
extern unsigned int distance, prevDistance;
extern bool serial;

void publishFeeds();

#endif