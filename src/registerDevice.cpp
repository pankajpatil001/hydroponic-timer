#include "registerDevice.h"

const char* registrationPath = "/api/register-park-slot-device";

bool isDeviceRegistered() {
    uint8_t flag = EEPROM.read(REGISTRATION_FLAG_ADDR);
    if (serial) Serial.print("Registration flag: ");
    if (serial) Serial.println(flag);
    if (flag == 0) if (serial) Serial.println("Device not registered.");
    if (flag == 1) if (serial) Serial.println("Device already registered.");
    return flag == 1;
}

void saveRegistrationStatus(const String& uuid) {
    if (serial) Serial.println("Saving registration status to EEPROM...");
    EEPROM.write(REGISTRATION_FLAG_ADDR, 1); // Set registration flag
    // EEPROM.put(UUID_START_ADDR, uuid); // Save UUID to EEPROM
    for (unsigned int i = 0; i < uuid.length(); i++) {
        EEPROM.write(UUID_START_ADDR + i, uuid[i]);
    }
    EEPROM.write(UUID_START_ADDR + uuid.length(), 0); // Null-terminate manually
    EEPROM.commit(); // Commit changes to EEPROM
    if (serial) Serial.println("Registration status saved.");
    if (serial) Serial.print("Device UUID: ");
    if (serial) Serial.println(uuid);
    if (serial) Serial.println("Registration flag set to 1.");
}

String getDeviceUUID() {
    bool eepromBlank = true;
    String uuid;
    for (int i = UUID_START_ADDR; i < UUID_START_ADDR + UUID_LENGTH; i++) {
        char c = EEPROM.read(i);
        if (c != 0xFF) eepromBlank = false; // Check if EEPROM is not blank
        if (c == 0) break; // Stop at null terminator
        uuid += c;
    }

    if (eepromBlank || uuid.length() == 0) {
        if (serial) Serial.println("EEPROM is blank or UUID not found.");
        return defaultUUID; // Return empty string if EEPROM is blank or UUID not found
    }
    return uuid;
}

bool registerDevice() {
    if (serial) Serial.println("Registering Device...");
    if (!wificlient.connect(SERVER, RPI_HTTP_PORT)) {
        if (serial) Serial.println("Failed to connect to server for registration.");
        return false;
    }

    if (deviceName[0] == '\0') {
        if (serial) Serial.println("Device name is empty. Cannot register.");
        if (serial) Serial.println("Complete the device setup first by visiting:.");
        if (serial) {
            Serial.print("http://");
            Serial.print(WiFi.localIP());
            Serial.print("/devicesetup/");
            Serial.println(defaultUUID);
        }
        return false;
    }

    String mac = WiFi.macAddress();
    String payload = "{\"park_slot_name\":\"" + String(deviceName) + 
                     "\",\"ps_mac_address\":\"" + mac +  
                     "\",\"firmware_version\":\"" + String(firmwareVersion) + 
                     "\",\"ps_device_ip\":\"" + WiFi.localIP().toString() + "\"}";

    String request = "POST " + String(registrationPath) + " HTTP/1.1\r\n" +
                     "Host: " + String(SERVER) + ":" + String(RPI_HTTP_PORT) + "\r\n" +
                     "Content-Type: application/json\r\n" +
                     "Content-Length: " + String(payload.length()) + "\r\n" +
                     "Connection: close\r\n\r\n" +
                     payload;
    wificlient.print(request);
    wificlient.flush(); 
    delay(100);

    if (serial) Serial.println("-----------------"); // Print separator
    //wait for server response
    // while (wificlient.available()) {
    //     String line = wificlient.readStringUntil('\n');
    //     if (line.length() == 0) break; // End of headers
    //     if (line == "\r" || line == "") break;
    //     if (serial) Serial.println(line); // Print server response
    // }
    while (wificlient.connected()) {
        String line = wificlient.readStringUntil('\n');
        if (line == "\r" || line.length() == 0) break;
        if (serial) Serial.println(line);
    }
    if (serial) Serial.println("-----------------"); // Print separator

    Serial.println("Waiting for server response...");
    String response = wificlient.readString();
    Serial.println("Server response: ");
    Serial.println(response); // Print server response

    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, response);

    if (error) {
        if (serial) Serial.print("❌ JSON parse error: ");
        if (serial) Serial.println(error.c_str());
        return false;
    }

    Serial.println("Parsed JSON document:");
    serializeJsonPretty(doc, Serial);
    Serial.println();  // Just for clarity in serial output

    if (doc.containsKey("device_uuid")) {
        String device_uuid = doc["device_uuid"];
        saveRegistrationStatus(device_uuid);
        if (serial) Serial.println("✅ Registered with device_uuid: " + device_uuid);
        return true;
    }

    if (serial) Serial.println("❌ UUID not found in response");

    return false;
}
