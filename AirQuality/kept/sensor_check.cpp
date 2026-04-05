#include <Arduino.h>

HardwareSerial pmsSerial(2);

void setup() {
  Serial.begin(115200);
  pmsSerial.begin(9600, SERIAL_8N1, 16, 17); 
  Serial.println("\n--- Stethoscope Mode (Raw Hex) ---");
}

void loop() {
  if (pmsSerial.available()) {
    // ถ้ามีข้อมูลมาแม้แต่ตัวเดียว ให้พ่นออกมาเป็นเลขฐาน 16 (Hex)
    Serial.printf("%02X ", pmsSerial.read());
  }
}