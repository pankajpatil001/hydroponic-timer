#include "callback.h"
//-----------------------------Callback-------------------------------
bool isValidInteger(const char* str) {
  if (str == nullptr || *str == '\0') return false;  // empty string
  int i = 0;

  if (str[0] == '-' || str[0] == '+') i++;  // allow sign

  for (; str[i] != '\0'; i++) {
    if (!isdigit(str[i])) return false;     // non-digit found
  }

  return true;
}

void callback(char* topic, byte * data, unsigned int length) {
  Serial.print("Incoming message on topic: ");
  Serial.println(topic);
  char onTopic[MQTT_USERNAME_SIZE + FEED_SIZE + 2]; // +2 FOR / AND \0
  strcpy(onTopic, mqttUsername);
  strcat(onTopic, "/");
  strcat(onTopic, onTimeFeed);
  Serial.print("Topic name: ");
  Serial.println(onTopic);
  char offTopic[MQTT_USERNAME_SIZE + FEED_SIZE + 2]; // +2 FOR / AND \0
  strcpy(offTopic, mqttUsername);
  strcat(offTopic, "/");
  strcat(offTopic, offTimeFeed);
  Serial.print("Topic name: ");
  Serial.println(offTopic);
  
  char msg[length + 1];          // +1 for null terminator
  memcpy(msg, data, length);  // Copy payload bytes
  msg[length] = '\0';            // Null-terminate

  Serial.print("Received message: ");
  Serial.println(msg);
  
  if(strcmp(topic, onTopic)==0) {
    Serial.println("Matched...");
    // Validate that payload is a proper integer string
    if (isValidInteger(msg)) {
      onTime = String(msg).toInt();
      Serial.print("Valid integer: "); Serial.println(onTime);
      Serial.println("Updated on time successfully!");

    } else {
      Serial.println("⚠️ Invalid integer payload! Could not update on time");
    }
  }
  else if(strcmp(topic, offTopic)==0) {
    Serial.println("Matched...");
    // Validate that payload is a proper integer string
    if (isValidInteger(msg)) {
      offTime = String(msg).toInt();
      Serial.print("Valid integer: "); Serial.println(offTime);
      Serial.println("Updated off time successfully!");

    } else {
      Serial.println("⚠️ Invalid integer payload! Could not update off time");
    }
  }
}