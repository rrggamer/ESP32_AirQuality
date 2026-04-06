#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_AHTX0.h>
#include <DFRobot_ENS160.h>

// Sensor object declarations
Adafruit_BMP280 bmp; 
Adafruit_AHTX0 aht;
DFRobot_ENS160_I2C ens(&Wire, 0x53); // Set ENS160 to address 0x53

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n--- Initializing Sensors ---");

  // Initialize I2C
  Wire.begin(21, 22);

  // 1. Initialize BMP280 (Address 0x76)
  if (!bmp.begin(0x76)) {
    Serial.println("Error: BMP280 not found!");
  } else {
    Serial.println("BMP280 Initialized successfully");
    // Set specific sampling parameters for internal weather
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL, 
                    Adafruit_BMP280::SAMPLING_X2, 
                    Adafruit_BMP280::SAMPLING_X16, 
                    Adafruit_BMP280::FILTER_X16,   
                    Adafruit_BMP280::STANDBY_MS_500);
  }

  // 2. Initialize AHT21 (Address 0x38)
  if (!aht.begin()) {
    Serial.println("Error: AHT21 not found!");
  } else {
    Serial.println("AHT21 Initialized successfully");
  }

  // 3. Initialize ENS160 (Address 0x53)
  if (ens.begin() != 0) {
    Serial.println("Error: ENS160 not found!");
  } else {
    Serial.println("ENS160 Initialized successfully");
    // Set operation mode to standard
    ens.setPWRMode(ENS160_STANDARD_MODE);
  }
}

void loop() {
  Serial.println("\n=============================");
  
  // Read AHT21 (Humidity & Temp)
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  Serial.printf("AHT21 - Temp: %.2f C | Humidity: %.2f %%\n", temp.temperature, humidity.relative_humidity);

  // Read BMP280 (Pressure)
  float pressure = bmp.readPressure() / 100.0F; // Convert Pa to hPa
  Serial.printf("BMP280 - Pressure: %.2f hPa\n", pressure);

  // Read ENS160 (Air Quality)
  ens.setTempAndHum(temp.temperature, humidity.relative_humidity); // Feed AHT21 data for compensation
  uint8_t aqi = ens.getAQI();
  uint16_t tvoc = ens.getTVOC();
  uint16_t eco2 = ens.getECO2();
  Serial.printf("ENS160 - AQI: %d | TVOC: %d ppb | eCO2: %d ppm\n", aqi, tvoc, eco2);

  delay(2000); // Read every 2 seconds
}