#ifndef WEB_SYSTEM_H
#define WEB_SYSTEM_H

#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>
#include <ESPmDNS.h>
#include <time.h>
#include "credentials.h"
#include "index_html.h"

extern AsyncWebServer server;
extern int current_pm25;

// Replace placeholders in HTML during initial load
String processor(const String& var){
  if(var == "PM25") {
    return String(current_pm25);
  }
  return String();
}

void setupNetwork() {
  // 1. WiFi Configuration
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi Connected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // 2. Time Synchronization (NTP)
  configTime(7 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  // 3. mDNS Setup (http://ottersense.local)
  if (MDNS.begin("ottersense")) {
    MDNS.addService("http", "tcp", 80);
  }

  // 4. Routes
  
  // Serve Main Dashboard
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // API for Auto-Refresh (JSON format)
  server.on("/api/data", HTTP_GET, [](AsyncWebServerRequest *request){
    struct tm timeinfo;
    String timeStr = "00:00:00";
    
    if(getLocalTime(&timeinfo)){
      char timeStringBuff[50];
      strftime(timeStringBuff, sizeof(timeStringBuff), "%H:%M:%S", &timeinfo);
      timeStr = String(timeStringBuff);
    }

    String json = "{\"pm25\":" + String(current_pm25) + ", \"time\":\"" + timeStr + "\"}";
    request->send(200, "application/json", json);
  });

  // 5. OTA System
  ElegantOTA.begin(&server);
  ElegantOTA.setAutoReboot(false); 
  
  ElegantOTA.onEnd([](bool success) {
    if (success) {
      Serial.println("OTA Success! Rebooting...");
      delay(2000); 
      ESP.restart();
    }
  });

  server.begin();
  Serial.println("HTTP Server started");
}

#endif