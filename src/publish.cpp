#include "publish.h"

void publishFeeds(){
  //------------------------RSSI-----------------------------
  if (millis() - tkeepRssi > rssiTime && client.connected()) {
    String str = "Test Device IP "; 
    str+= WiFi.localIP().toString();
    str+= ", RSSI ";
    str+= (String)WiFi.RSSI();
    str+= " dBm";
    str.toCharArray(valueStr, 70);
//    Serial.println(str);
    client.publish(PREAMBLE MESSAGES, valueStr);
    tkeepRssi = millis();
  }

  // ------------------------Ultrasonic Sensor---------------------------
  // if (millis() - tkeepUSpub > uspubTime && client.connected() && prevDistance != distance) {
  if (millis() - tkeepUSpub > uspubTime && client.connected()) {

    // String str = "Water level is "; 
    String str = (String)distance;
    // str+= " cm\n";
    str.toCharArray(valueStr, 10);
//    Serial.println(str);
    if(distance < 500) client.publish(PREAMBLE PARKINDCTR, valueStr);
    // prevDistance = distance;
    tkeepUSpub = millis();
  }
}