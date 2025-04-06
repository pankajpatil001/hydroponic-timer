#ifndef ultrasonic_h
#define ultrasonic_h
#include "Arduino.h"
#include "defines.h"

#define MAX_SAMPLES 20

extern unsigned long duration;
extern bool serial, parkSpaceOccupied;
extern unsigned int distance, prevDistance;
extern unsigned long tkeepUS;
extern unsigned int usTime, parkSpaceVehicleDistance;
void getDistance();

#endif