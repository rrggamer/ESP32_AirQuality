#include <Arduino.h>
#include <GxEPD2_BW.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>
#include "credentials.h"

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n--- OtterSense S3 Booting ---");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "OtterSense S3 is Online! Go to /update for OTA.");
  });

  ElegantOTA.begin(&server); 
  
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
}