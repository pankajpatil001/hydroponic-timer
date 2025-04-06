#ifndef callback_h
#define callback_h
#include "Arduino.h"
#include <EEPROM.h>
#include "defines.h"

extern bool serial;
extern uint8_t wifiReconnectAttemptCount, addr_L1, addr_L, addr_security, addr_M, minLight, maxLight;
extern byte stat_L1, stat_L, security_state, stat_M;
extern String incoming[100];

void callback(char* topic, byte * data, unsigned int length);

#endif