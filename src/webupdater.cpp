#include "webupdater.h"


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

void WiFi_httpStuff(){

  // WiFiManager handles the connection process
  WiFiManager wifiManager;
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  // Add a custom parameter
  WiFiManagerParameter custom_mqtt_server("server", "MQTT Server IP", mqttServer, 40);
  wifiManager.addParameter(&custom_mqtt_server);


  // Blocks until connected or credentials provided via captive portal
  if (!wifiManager.autoConnect(ACCESS_POINT_NAME, ACCESS_POINT_PWD)) {
    Serial.println("WiFi failed to connect, restarting...");
    delay(3000);
    ESP.restart();
  }

  if (configFreshlySaved) {
    // Portal was used; save user inputs to EEPROM
    strncpy(mqttServer, custom_mqtt_server.getValue(), sizeof(mqttServer));
    EEPROM.put(0, mqttServer);  // Write starting at address 0
    EEPROM.commit();

    Serial.print("MQTT Server saved: ");
    Serial.println(mqttServer);
    Serial.println("Custom MQTT Server saved from portal input");
  } else {
    Serial.println("Using saved WiFi credentials; skipping EEPROM write");
    EEPROM.get(0, mqttServer);
    Serial.print("Loaded MQTT Server from EEPROM: ");
    Serial.println(mqttServer);  
    if (strlen(mqttServer) == 0 || mqttServer[0] == 0xFF) {
      // EEPROM is empty or uninitialized
      Serial.println("EEPROM empty, using fallback MQTT server IP");
      strcpy(mqttServer, SERVER);  // fallback value
    
      // Optional: trigger an API call here to fetch actual IP
      // OR start WiFiManager portal to let user enter it
    } else strcpy(SERVER, mqttServer); // Use the loaded value for MQTT server
  }
  
  WiFi.mode(WIFI_STA);

  httpUpdater.setup(&httpServer);

  httpServer.on("/", HTTP_GET, []() {
    httpServer.sendHeader("Connection", "close");
    httpServer.send(200, "text/html", loginIndex);
  });
  httpServer.on("/triggerUpdate", HTTP_GET, []() {
    httpServer.send(200, "text/plain", "Starting OTA...");
    performOTAUpdate();  // This will reboot if successful
  });
  httpServer.on("/register", HTTP_GET, []() {
    httpServer.send(200, "text/plain", "Register...");
    registerDevice();  
  });
  httpServer.on("/serverIndex", HTTP_GET, []() {
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
  
  httpServer.begin();
  
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