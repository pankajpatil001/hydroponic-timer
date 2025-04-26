#include "publish.h"

void publishFeeds(){
  //------------------------RSSI-----------------------------
  if (millis() - tkeepRssi > rssiTime && client.connected()) {
    String str = "Parking Sensor Device V2.2 IP "; 
    str+= WiFi.localIP().toString();
    str+= ", RSSI ";
    str+= (String)WiFi.RSSI();
    str+= " dBm";
    str.toCharArray(valueStr, 70);
   Serial.println(str);
    client.publish(PREAMBLE MESSAGES, valueStr);
    tkeepRssi = millis();
  }

  // ------------------------Ultrasonic Sensor---------------------------
  // if (millis() - tkeepUSpub > uspubTime && client.connected() && prevDistance != distance) {
  if (millis() - tkeepUSpub > uspubTime && client.connected()) {

    char topic[256];
    snprintf(topic, sizeof(topic), "%s%s%s", PREAMBLE, DISTANCE, deviceUUID);
    // String str = "Water level is "; 
    String str = (String)distance;
    // str+= " cm\n";
    str.toCharArray(valueStr, 10);
//    Serial.println(str);
    if (serial) Serial.print("Publishing to topic: ");
    if (serial) Serial.println(topic);
    if (serial) Serial.print("Distance: ");
    if (serial) Serial.println(str);
    if(distance < 500) client.publish(topic, valueStr);
    // prevDistance = distance;
    tkeepUSpub = millis();
  }
}