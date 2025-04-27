#include "connectSubscribe.h"

void connectSubscribe(){
  if (!client.connected() && millis() - tkeepConnect > connectTime) {
    if (WiFi.status() != WL_CONNECTED) {

      if (wifiConnected) {
        Serial.println("⚠️ WiFi lost!");
        wifiConnected = false;
        lastReconnectAttempt = millis();
      }

      // Try to reconnect every 10 seconds
      if (millis() - lastReconnectAttempt > 10000) {
        Serial.println("🔄 Attempting WiFi reconnect...");
        WiFi.reconnect();
        lastReconnectAttempt = millis();
      }
      // wifiMulti.run();
      // delay(1000);
      wifiReconnectAttemptCount++;
      if(wifiReconnectAttemptCount>5) {
        if (serial) Serial.println("WiFi reconnect failed. Restarting...");
        delay(1000);
        ESP.restart();
      }
    }
    else {
      wifiReconnectAttemptCount=0;
      if(serial) Serial.println("Attempting MQTT connection...");
      // Attempt to connect
      if (client.connect("", mqttUsername, mqttKey)) {
        if(serial) Serial.println("connected");
        // ... and resubscribe
  
        delay(1);
        connection = HIGH;
        // getFeedLatest(); //get latest values of feed
      } 
      else {
        if(serial) Serial.print("Failed, rc=");
        if(serial) Serial.print(client.state());
        if(serial) Serial.println(" Retrying soon...!!!");
      }
    }
    tkeepConnect = millis();
  }
}

void checkConnection(){
    //----Connection LED-----
  if(client.connected() && connection == LOW) {
    wifiReconnectAttemptCount=0;
    digitalWrite(LED, LOW); //Connected
    connection = HIGH;
  }
  else if(!client.connected() && connection == HIGH){
    digitalWrite(LED, HIGH); //Not Connected
    connection = LOW;
  }
  //-----------------------
}