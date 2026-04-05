#include <Arduino.h>
#include "web_system.h"


AsyncWebServer server(80);
int current_pm25 = 45; 

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  setupNetwork();
  
  Serial.println("System Ready!");
}

void loop() {

}