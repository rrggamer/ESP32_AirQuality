#include <Arduino.h>
#include "web_system.h"
#include "pms_sensor.h"

// Global server and data variables
AsyncWebServer server(80);
int current_pm25 = 0; 
unsigned long lastReadTime = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Initialize Network and OTA services
  setupNetwork();

  // Initialize PMS5003 Sensor
  setupPMS();
  
  Serial.println("System initialized successfully");
}

void loop() {
  // Read sensor data in non-blocking mode
  int val = readPM25();
  
  // Update global variable if valid data is received
  if (val != -1) {
    current_pm25 = val;
    
    // Serial debug output every 5 seconds
    if (millis() - lastReadTime > 5000) {
      Serial.printf("Current PM2.5: %d ug/m3\n", current_pm25);
      lastReadTime = millis();
    }
  }
}