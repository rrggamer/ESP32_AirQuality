#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_AHTX0.h>
#include <DFRobot_ENS160.h>
#include "web_system.h"
#include "pms_sensor.h"

// Global Variables
AsyncWebServer server(80);
int current_pm25 = 0;
float current_temp = 0, current_hum = 0, current_pres = 0;
uint16_t current_eco2 = 0, current_tvoc = 0;

// Sensor Objects
Adafruit_BMP280 bmp;
Adafruit_AHTX0 aht;
DFRobot_ENS160_I2C ens(&Wire, 0x53);

// Timer variable for non-blocking delay
unsigned long lastI2CRead = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  // Initialize sensors
  setupPMS();
  if (!bmp.begin(0x76)) Serial.println("BMP280 failed");
  if (!aht.begin()) Serial.println("AHT21 failed");
  if (ens.begin() != 0) Serial.println("ENS160 failed");
  else ens.setPWRMode(ENS160_STANDARD_MODE);

  setupNetwork();
  Serial.println("OtterSense Pro Ready");
}

void loop() {
  // 1. Read PM2.5 continuously (No delays here!)
  // This ensures the Serial buffer is always emptied and parsed correctly.
  int pm_val = readPM25();
  if (pm_val != -1) {
    current_pm25 = pm_val;
  }

  // 2. Read I2C sensors only every 2000 milliseconds (Non-blocking)
  if (millis() - lastI2CRead >= 2000) {
    lastI2CRead = millis(); // Reset the timer

    // Read Environment (AHT21 & BMP280)
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);
    current_temp = temp.temperature;
    current_hum = humidity.relative_humidity;
    current_pres = bmp.readPressure() / 100.0F;

    // Read Gas (ENS160)
    ens.setTempAndHum(current_temp, current_hum);
    current_eco2 = ens.getECO2();
    current_tvoc = ens.getTVOC();
  }
}