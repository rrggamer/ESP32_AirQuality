#include <Arduino.h>
#include "web_system.h"
#include "pms_sensor.h"

AsyncWebServer server(80);
int current_pm25 = 0;
unsigned long lastReadTime = 0; 

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  setupNetwork();
  
  setupPMS();

  Serial.println("System Ready!");
}

void loop() {
  
  int val = readPM25();
  
  
  if (val != -1) {
    current_pm25 = val;
    
   
    if (millis() - lastReadTime > 2000) {
      Serial.printf("Live PM2.5: %d ug/m3\n", current_pm25);
      lastReadTime = millis();
    }
  }
}