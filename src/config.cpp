#include "config.h"

// Helper to read char array from EEPROM
void readFromEEPROM(int startAddr, char* dest, int size) {
    bool allFF = true;
    for (int i = 0; i < size; i++) {
        byte val = EEPROM.read(startAddr + i);
        dest[i] = val;
        if (val != 0xFF && val != 0x00) {  // valid character
          allFF = false;
        }
    }
    dest[size - 1] = '\0'; // Null-terminate

    if (allFF) {
      strcpy(dest, ""); // <<< whatever default you want
    }
  }
  
  // Helper to write char array to EEPROM
  void writeToEEPROM(int startAddr, const char* src, int size) {
    for (int i = 0; i < size; i++) {
      EEPROM.write(startAddr + i, src[i]);
    }
  }
  
  // Load config values from EEPROM
  void loadConfig() {
    readFromEEPROM(RPI_IP_ADDR, rpiServer, RPI_IP_SIZE);
    readFromEEPROM(DEVICE_NAME_ADDR, deviceName, DEVICE_NAME_SIZE);
    readFromEEPROM(MQTT_USERNAME_ADDR, mqttUsername, MQTT_USERNAME_SIZE);
    readFromEEPROM(MQTT_KEY_ADDR, mqttKey, MQTT_KEY_SIZE);
  }
  
  // Save config values to EEPROM
  void saveConfig() {
    writeToEEPROM(RPI_IP_ADDR, rpiServer, RPI_IP_SIZE);
    writeToEEPROM(DEVICE_NAME_ADDR, deviceName, DEVICE_NAME_SIZE);
    writeToEEPROM(MQTT_USERNAME_ADDR, mqttUsername, MQTT_USERNAME_SIZE);
    writeToEEPROM(MQTT_KEY_ADDR, mqttKey, MQTT_KEY_SIZE);
  
    EEPROM.commit();
  }

//   void initializeConfig() {
//     loadConfig(); // Load saved config at boot

//     Serial.println("Loaded Configuration:");
//     Serial.print("RPI IP Address: "); Serial.println(rpiServer);
//     Serial.print("Device Name: "); Serial.println(deviceName);
//     Serial.print("MQTT Username: "); Serial.println(mqttUsername);
//     Serial.print("MQTT Key: "); Serial.println(mqttKey);

//     saveConfig(); // Save updated config
//   }