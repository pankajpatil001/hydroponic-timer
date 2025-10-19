#include "callback.h"
//-----------------------------Callback-------------------------------

void callback(char* topic, byte * data, unsigned int length) {
  Serial.print("Incoming message on topic: ");
  Serial.println(topic);

  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)data[i];
  }

  Serial.print("Payload: ");
  Serial.println(message);
  if(strcmp(topic, ON_TIME)==0) {
    if(serial) Serial.print(topic);
    if(serial) Serial.print(" : ");
    for (unsigned int i = 0; i < length; i++) {
      incoming[i] = (char)data[i];
      if(serial) Serial.print((char)data[i]);
    }
    if(serial) Serial.println();
  }
  else if(strcmp(topic, OFF_TIME)==0) {
    if(serial) Serial.print(topic);
    if(serial) Serial.print(" : ");
    for (unsigned int i = 0; i < length; i++) {
      incoming[i] = (char)data[i];
      if(serial) Serial.print((char)data[i]);
    }
    if(serial) Serial.println();
  }
}