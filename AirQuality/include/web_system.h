#ifndef WEB_SYSTEM_H
#define WEB_SYSTEM_H

#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>
#include <ESPmDNS.h>
#include <time.h>
#include <ArduinoJson.h>
#include "credentials.h"
#include "index_html.h"

extern AsyncWebServer server;

// Global sensor variables (defined in main.cpp)
extern int current_pm25;
extern float current_temp, current_hum, current_pres;
extern uint16_t current_eco2, current_tvoc;

void setupNetwork() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) { delay(500); }
  
  configTime(7 * 3600, 0, "pool.ntp.org");
  MDNS.begin("ottersense");

  // Main page route
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Expanded API route using ArduinoJson for clean formatting
  server.on("/api/data", HTTP_GET, [](AsyncWebServerRequest *request){
    struct tm timeinfo;
    char timeBuff[10] = "00:00:00";
    if(getLocalTime(&timeinfo)) {
      strftime(timeBuff, sizeof(timeBuff), "%H:%M:%S", &timeinfo);
    }

    StaticJsonDocument<256> doc;
    doc["pm25"] = current_pm25;
    doc["temp"] = current_temp;
    doc["hum"] = current_hum;
    doc["pres"] = current_pres;
    doc["eco2"] = current_eco2;
    doc["tvoc"] = current_tvoc;
    doc["time"] = timeBuff;

    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
  });

  ElegantOTA.begin(&server);
  server.begin();
}

#endif