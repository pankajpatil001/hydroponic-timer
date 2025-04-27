#include "webupdater.h"

String setUpForm() {
  String page = "<!DOCTYPE html><html><head><title>Device Config</title>";
  page += "<style>body{font-family:sans-serif;max-width:400px;margin:auto;padding:1em;}input{width:100%;margin-bottom:1em;padding:0.5em;}</style>";
  page += "</head><body><h2>Configure Device</h2><form method='POST' action='/save-device-setup'>";
  page += "<label>RPI IP Address:</label><input name='rpiServer' value='" + String(rpiServer) + "' required>";
  page += "<label>Device Name:</label><input name='deviceName' value='" + String(deviceName) + "' required>";
  page += "<label>MQTT Username:</label><input name='mqttUsername' value='" + String(mqttUsername) + "' required>";
  page += "<label>MQTT Password:</label><input name='mqttKey' value='" + String(mqttKey) + "' required>";
  page += "<input type='submit' value='Save'>";
  page += "</form></body></html>";
  return page;
}

void handleSaveDeviceSetup() {
  if (httpServer.hasArg("rpiServer")) {
    strncpy(rpiServer, httpServer.arg("rpiServer").c_str(), RPI_IP_SIZE);
    rpiServer[RPI_IP_SIZE - 1] = '\0';
  }
  if (httpServer.hasArg("deviceName")) {
    strncpy(deviceName, httpServer.arg("deviceName").c_str(), DEVICE_NAME_SIZE);
    deviceName[DEVICE_NAME_SIZE - 1] = '\0';
  }
  if (httpServer.hasArg("mqttUsername")) {
    strncpy(mqttUsername, httpServer.arg("mqttUsername").c_str(), MQTT_USERNAME_SIZE);
    mqttUsername[MQTT_USERNAME_SIZE - 1] = '\0';
  }
  if (httpServer.hasArg("mqttKey")) {
    strncpy(mqttKey, httpServer.arg("mqttKey").c_str(), MQTT_KEY_SIZE);
    mqttKey[MQTT_KEY_SIZE - 1] = '\0';
  }

  saveConfig(); // Save to EEPROM

  httpServer.send(200, "text/html", "<h2>Config saved!</h2><p>Device will restart now.</p>");
  delay(1000);
  ESP.restart();
}
void resetToFactorySettings() {
  if(serial) Serial.println("🔄 Resetting to factory settings...");
  WiFiManager wifiManager;
  wifiManager.resetSettings();
  for (int i = 0; i < EEPROM_SIZE; i++) {
    EEPROM.write(i, 0xFF); // similar to new EEPROM
  }
  EEPROM.commit(); // Commit changes to EEPROM
  if(serial) Serial.println("🔄 Factory settings reset. Rebooting...");
  delay(1000);
  ESP.restart();
}

void performOTAUpdate() {
  Serial.println("🔄 Starting OTA update...");

  wificlientsecure.setInsecure(); // Disable SSL certificate verification (not recommended for production)

  t_httpUpdate_return ret = ESPhttpUpdate.update(wificlientsecure, firmwareURL);

  switch (ret) {
    case HTTP_UPDATE_FAILED:
      Serial.printf("❌ OTA failed. Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
      break;

    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("✔️ No update available.");
      break;

    case HTTP_UPDATE_OK:
      Serial.println("✅ Update successful, rebooting...");
      break;
  }
}

void saveConfigCallback() {
  if(serial) Serial.println("WiFiManager config freshly saved...!!!");
  configFreshlySaved = HIGH;
}

void startHTTPServer() {
  if (serial) Serial.println("Starting HTTP server...");
  httpServer.begin();
  if (serial) Serial.println("HTTP server started.");
}

void setupHTTPRoutes() {
  if (serial) Serial.println("Setting up HTTP routes...");
  
  String loginPath = String("/login/") + String(deviceUUID);
  String triggerUpdatePath = String("/triggerUpdate/") + String(deviceUUID);
  String registerPath = String("/register/") + String(deviceUUID);
  String resetFactorySettingsPath = String("/reset-to-factory-settings/") + String(deviceUUID);
  String deviceSetupPath = String("/devicesetup/") + String(deviceUUID);
  String saveDeviceSetupPath = String("/save-device-setup/") + String(deviceUUID);
  String otaUpdatePath = String("/ota-update/") + String(deviceUUID);
  // String loginPath = String("/login/") + String(deviceUUID);

  httpServer.on(loginPath.c_str(), HTTP_GET, []() {
    if (serial) Serial.println("Login page requested.");
    httpServer.sendHeader("Connection", "close");
    httpServer.send(200, "text/html", loginIndex);
  });
  httpServer.on(triggerUpdatePath, HTTP_GET, []() {
    httpServer.sendHeader("Connection", "close");
    httpServer.send(200, "text/plain", "Starting OTA...");
    performOTAUpdate();  // This will reboot if successful
  });
  httpServer.on(registerPath, HTTP_GET, []() {
    httpServer.sendHeader("Connection", "close");
    httpServer.send(200, "text/plain", "Register...");
    registerDevice();  
  });
  httpServer.on(resetFactorySettingsPath, HTTP_GET, []() {
    if (!httpServer.authenticate(OTA_USERNAME, OTA_PASSWORD)) {
      return httpServer.requestAuthentication();
    }
    httpServer.sendHeader("Connection", "close");
    httpServer.send(200, "text/plain", "Resetting to factory settings...");
    resetToFactorySettings();  
  });
  httpServer.on(deviceSetupPath, HTTP_GET, []() {
    if (!httpServer.authenticate(OTA_USERNAME, OTA_PASSWORD)) {
      return httpServer.requestAuthentication();
    }
    httpServer.sendHeader("Connection", "close");
    httpServer.send(200, "text/html", setUpForm());
  });
  httpServer.on(saveDeviceSetupPath, HTTP_POST, handleSaveDeviceSetup);
  httpServer.on(otaUpdatePath, HTTP_GET, []() {
    if (!httpServer.authenticate(OTA_USERNAME, OTA_PASSWORD)) {
      return httpServer.requestAuthentication();
    }
    httpServer.sendHeader("Connection", "close");
    httpServer.send(200, "text/html", serverIndex);
  });
  /*handling uploading firmware file */
  httpServer.on("/update", HTTP_POST, []() {
    if (!httpServer.authenticate(OTA_USERNAME, OTA_PASSWORD)) {
      return httpServer.requestAuthentication();
    }
    httpServer.sendHeader("Connection", "close");
    httpServer.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    // HTTPUpload& upload = httpServer.upload();
    httpServer.upload();
  });

  if (serial) Serial.println("HTTP routes set up successfully.");
}

void WiFi_httpStuff(){

  // WiFiManager handles the connection process
  WiFiManager wifiManager;
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  // Add a custom parameter
  WiFiManagerParameter custom_mqtt_server("server", "MQTT Server IP", rpiServer, 40);
  wifiManager.addParameter(&custom_mqtt_server);

  // Blocks until connected or credentials provided via captive portal
  if (!wifiManager.autoConnect(ACCESS_POINT_NAME, ACCESS_POINT_PWD)) {
    Serial.println("WiFi failed to connect, restarting...");
    delay(3000);
    ESP.restart();
  }

  if (configFreshlySaved) {
    // Portal was used; save user inputs to EEPROM
    strncpy(rpiServer, custom_mqtt_server.getValue(), sizeof(rpiServer));
    EEPROM.put(RPI_IP_ADDR, rpiServer);  // Write starting at address 0
    EEPROM.commit();

    Serial.print("MQTT Server saved: ");
    Serial.println(rpiServer);
    Serial.println("Custom MQTT Server saved from portal input");
  } else {
    Serial.println("Using saved WiFi credentials; skipping EEPROM write");
    EEPROM.get(RPI_IP_ADDR, rpiServer);
    Serial.print("Loaded MQTT Server from EEPROM: ");
    Serial.println(rpiServer);  
    if (strlen(rpiServer) == 0 || rpiServer[0] == 0xFF) {
      // EEPROM is empty or uninitialized
      Serial.println("EEPROM empty, using fallback MQTT server IP");
      strcpy(rpiServer, SERVER);  // fallback value
    
      // Optional: trigger an API call here to fetch actual IP
      // OR start WiFiManager portal to let user enter it
    } else strcpy(SERVER, rpiServer); // Use the loaded value for MQTT server
  }
  
  WiFi.mode(WIFI_STA);

  httpUpdater.setup(&httpServer);
  
  // httpServer.begin(); // this is not needed here
  
  // MDNS.addService("http", "tcp", 80);
  if(!wifiConnected && WiFi.status() == WL_CONNECTED) {
      wifiConnected = true;
      if(serial) Serial.println("WiFi connected...!!!");
      if(serial) Serial.println("IP address: ");
      if(serial) Serial.println(WiFi.localIP());
      
      if(serial) Serial.print("HTTPUpdateServer ready! Open http://");
      if(serial) Serial.print(WiFi.localIP());
      if(serial) Serial.println("/ in your browser.");
  }
  else {
    if(serial) Serial.println("WiFi not connected...!!!");
  }
}