#ifndef WEB_SYSTEM_H
#define WEB_SYSTEM_H

#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>
#include <ESPmDNS.h>
#include "credentials.h"
#include "index_html.h"

extern AsyncWebServer server; 
extern int current_pm25;     

// ฟังก์ชันจัดการตัวแปรใน HTML
String processor(const String& var){
  if(var == "PM25") return String(current_pm25);
  return String();
}

void setupNetwork() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  
  Serial.println("\nIP: " + WiFi.localIP().toString());

  if (MDNS.begin("ottersense")) {
    MDNS.addService("http", "tcp", 80);
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  ElegantOTA.begin(&server);
  
  
  ElegantOTA.setAutoReboot(false); 
  ElegantOTA.onEnd([](bool success) {
    if (success) {
      Serial.println("OTA Done! Rebooting in 2s...");
      delay(2000);
      ESP.restart();
    }
  });

  server.begin();
}

#endif