#include "timer.h"

void monitorTimer(){
  
  //------------------------RSSI-----------------------------
  if (millis() - tkeepTimer > timerTime && client.connected()) {
    Serial.println("Checking Timer...");

    if (waterPumpOn && millis() - timer > onTime * 1000) {
        Serial.println("Water Pump turned off...");
        digitalWrite(waterPumpPin, LOW);
        waterPumpOn = false;
        char msgsTopic[MQTT_USERNAME_SIZE + FEED_SIZE + 2]; // +2 FOR / AND \0
        String str = "Hydroponic Water Pump turned off...! ";
        str.toCharArray(valueStr, 70);
        strcpy(msgsTopic, mqttUsername);
        strcat(msgsTopic, "/");
        strcat(msgsTopic, msgsFeed);
        Serial.print("Topic name: ");
        Serial.println(msgsTopic);
        // Serial.println(MESSAGES);
        if (client.publish(msgsTopic, valueStr)) Serial.println("Successfully published.");
        timer = millis();
    } else if (!waterPumpOn && millis() - timer > offTime * 1000) {
        Serial.println("Water Pump turned on...");
        digitalWrite(waterPumpPin, HIGH);
        waterPumpOn = true;
        char msgsTopic[MQTT_USERNAME_SIZE + FEED_SIZE + 2]; // +2 FOR / AND \0
        String str = "Hydroponic Water Pump turned on...! ";
        str.toCharArray(valueStr, 70);
        strcpy(msgsTopic, mqttUsername);
        strcat(msgsTopic, "/");
        strcat(msgsTopic, msgsFeed);
        Serial.print("Topic name: ");
        Serial.println(msgsTopic);
        // Serial.println(MESSAGES);
        if (client.publish(msgsTopic, valueStr)) Serial.println("Successfully published.");
        timer = millis();
    }
    tkeepTimer = millis();
  }

}