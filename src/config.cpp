#include "config.h"

// Helper to read char array from EEPROM
void readFromEEPROM(int startAddr, char* dest, int size) {
    bool allFF = true;
    Serial.println("Read from eeprom:");
    for (int i = 0; i < size; i++) {
        byte val = EEPROM.read(startAddr + i);
        dest[i] = val;
        // Serial.print(val);
        if (val != 0xFF && val != 0x00) {  // valid character
          allFF = false;
        }
    }
    dest[size - 1] = '\0'; // Null-terminate

    if (allFF) {
      strcpy(dest, ""); // <<< whatever default you want
    }
    
    Serial.println(dest);
  }
  
  // Helper to write char array to EEPROM
  void writeToEEPROM(int startAddr, const char* src, int size) {
    Serial.println("Writing to eeprom:");
    for (int i = 0; i < size; i++) {
      // Serial.print(src[i]);
      EEPROM.write(startAddr + i, src[i]);
    }
    Serial.println(src);
  }
  
  // Load config values from EEPROM
  void loadConfig() {
    readFromEEPROM(RPI_IP_ADDR, rpiServer, RPI_IP_SIZE);
    readFromEEPROM(DEVICE_NAME_ADDR, deviceName, DEVICE_NAME_SIZE);
    readFromEEPROM(MQTT_USERNAME_ADDR, mqttUsername, MQTT_USERNAME_SIZE);
    readFromEEPROM(MQTT_KEY_ADDR, mqttKey, MQTT_KEY_SIZE);
    EEPROM.get(ON_TIME_ADDR, onTime);
    EEPROM.get(OFF_TIME_ADDR, offTime);
    EEPROM.get(TEST_TIME_ADDR, testTime);
    // readFromEEPROM(ON_TIME_ADDR, (char*)&onTime, TIME_SIZE);
    // readFromEEPROM(OFF_TIME_ADDR, (char*)&offTime, TIME_SIZE);
    // readFromEEPROM(TEST_TIME_ADDR, (char*)&testTime, TIME_SIZE);
    // readFromEEPROM(VEH_DIST_ADDR, (char*)&parkSpaceVehicleDistance, VEH_DIST_SIZE); // Read vehicle distance from EEPROM
    // parkSpaceVehicleDistance = (parkSpaceVehicleDistance < minVehDistance || parkSpaceVehicleDistance > maxVehDistance) ? 50 : parkSpaceVehicleDistance; // Default to 50 if not set
  }
  
  // Save config values to EEPROM
  void saveConfig() {
    writeToEEPROM(RPI_IP_ADDR, rpiServer, RPI_IP_SIZE);
    writeToEEPROM(DEVICE_NAME_ADDR, deviceName, DEVICE_NAME_SIZE);
    writeToEEPROM(MQTT_USERNAME_ADDR, mqttUsername, MQTT_USERNAME_SIZE);
    writeToEEPROM(MQTT_KEY_ADDR, mqttKey, MQTT_KEY_SIZE);
    EEPROM.put(ON_TIME_ADDR, onTime);
    EEPROM.put(OFF_TIME_ADDR, offTime);
    EEPROM.put(TEST_TIME_ADDR, testTime);
    // writeToEEPROM(ON_TIME_ADDR, (char*)&onTime, TIME_SIZE);
    // writeToEEPROM(OFF_TIME_ADDR, (char*)&offTime, TIME_SIZE);
    // writeToEEPROM(TEST_TIME_ADDR, (char*)&testTime, TIME_SIZE);
    // writeToEEPROM(VEH_DIST_ADDR, (char*)&parkSpaceVehicleDistance, VEH_DIST_SIZE); // Save vehicle distance to EEPROM
  
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