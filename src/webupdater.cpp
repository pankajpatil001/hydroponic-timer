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

void WiFi_httpStuff(){

  // WiFiManager handles the connection process
  WiFiManager wifiManager;

  // Blocks until connected or credentials provided via captive portal
  if (!wifiManager.autoConnect(ACCESS_POINT_NAME, ACCESS_POINT_PWD)) {
    Serial.println("WiFi failed to connect, restarting...");
    delay(3000);
    ESP.restart();
  }

  Serial.println("WiFi Connected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

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