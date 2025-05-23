#include "registerDevice.h"

const char* registrationPath = "/api/register-park-slot-device";
const char* updateIPPath = "/api/update-park-slot-ip";

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

bool updateDeviceIP() {
    if (serial) Serial.println("Updating Device IP...");

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
    String payload = "{\"ps_mac_address\":\"" + mac +  
                     "\",\"firmware_version\":\"" + String(firmwareVersion) + 
                     "\",\"ps_device_ip\":\"" + WiFi.localIP().toString() + "\"}";
                     
    if (serial) Serial.println("Sending update IP request:");
    String url = "http://" + String(rpiServer) + ":5000" + String(updateIPPath);
    if (serial) Serial.println("Posting to: " + url);
    if (serial) Serial.println("Payload: " + payload);
    
    rpihttp.begin(wificlient, url);
    rpihttp.addHeader("Content-Type", "application/json");

    int httpCode = rpihttp.POST(payload);
    String response = rpihttp.getString();

    if (serial) Serial.println("HTTP code: " + String(httpCode));
    if (serial) Serial.println("Response: " + response);

    rpihttp.end();

    if (httpCode == HTTP_CODE_OK) {
        StaticJsonDocument<128> doc;
        DeserializationError error = deserializeJson(doc, response);
        if (serial) Serial.println("Parsed JSON document:");
        serializeJsonPretty(doc, Serial);
        if (serial) Serial.println();  // Just for clarity in serial output

        if (!error && doc.containsKey("status")) {
            String updateIPStatus = doc["status"];
            if (serial) Serial.println("✅ Update Status: " + updateIPStatus);
            return true;
        } else {
            if (serial) Serial.println("❌ Failed to parse update status from response.");
            return false;
        }
    } else {
        if (serial) Serial.println("❌ HTTP request failed.");
        return false;
    }
}

bool registerDevice() {
    if (serial) Serial.println("Registering Device...");
    if (serial) {
        Serial.print("Default server: ");
        Serial.println(SERVER);
        Serial.print("Connecting to: ");
        Serial.println(rpiServer);
        Serial.print(" Port: ");
        Serial.println(RPI_HTTP_PORT);
    }

    // if (!wificlient.connect(rpiServer, RPI_HTTP_PORT)) {
    //     if (serial) Serial.println("Failed to connect to server for registration.");
    //     return false;
    // }

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

    if (serial) Serial.println("Sending registration request:");
    String url = "http://" + String(rpiServer) + ":5000" + String(registrationPath);
    Serial.println("Posting to: " + url);
    Serial.println("Payload: " + payload);
    
    rpihttp.begin(wificlient, url);
    rpihttp.addHeader("Content-Type", "application/json");

    int httpCode = rpihttp.POST(payload);
    String response = rpihttp.getString();

    Serial.println("HTTP code: " + String(httpCode));
    Serial.println("Response: " + response);

    rpihttp.end();

    if (httpCode == HTTP_CODE_OK) {
        StaticJsonDocument<128> doc;
        DeserializationError error = deserializeJson(doc, response);
        Serial.println("Parsed JSON document:");
        serializeJsonPretty(doc, Serial);
        Serial.println();  // Just for clarity in serial output

        if (!error && doc.containsKey("device_uuid")) {
            String device_uuid = doc["device_uuid"];
            Serial.println("✅ Received UUID: " + device_uuid);
            saveRegistrationStatus(device_uuid);
            if (serial) Serial.println("✅ Registered with device_uuid: " + device_uuid);
            return true;
        } else {
            Serial.println("❌ Failed to parse UUID from response.");
            return false;
        }
    } else {
        Serial.println("❌ HTTP request failed.");
        return false;
    }
}
