#include "timer.h"

void monitorTimer(){
  
  //------------------------RSSI-----------------------------
  if (millis() - tkeepTimer > timerTime && client.connected()) {
    Serial.println("Checking Timer...");

    if (waterPumpOn && millis() - timer > onTime * 1000) {
        Serial.println("Water Pump turned off...");
        digitalWrite(waterPumpPin, LOW);
        waterPumpOn = false;
        timer = millis();
    } else if (!waterPumpOn && millis() - timer > offTime * 1000) {
        Serial.println("Water Pump turned on...");
        digitalWrite(waterPumpPin, HIGH);
        waterPumpOn = true;
        timer = millis();
    }
    tkeepTimer = millis();
  }

}