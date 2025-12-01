#ifndef timer_h
#define timer_h
#include "Arduino.h"

#include "defines.h"
#include <PubSubClient.h>

extern bool waterPumpOn;
extern unsigned int onTime, offTime, timerTime;
extern unsigned long tkeepTimer, timer;
extern PubSubClient client;
extern char mqttUsername[MQTT_USERNAME_SIZE], msgsFeed[FEED_SIZE];
extern char valueStr[100];

void monitorTimer();

#endif