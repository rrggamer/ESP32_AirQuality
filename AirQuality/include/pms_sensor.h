#ifndef PMS_SENSOR_H
#define PMS_SENSOR_H

#include <Arduino.h>
#include "PMS.h"

// ใช้ Hardware Serial 1 ของ ESP32-S3
HardwareSerial pmsSerial(2);
PMS pms(pmsSerial);
PMS::DATA data;

void setupPMS() {
  // เริ่มต้น Serial1 ที่ Baud 9600 (RX=16, TX=17)
  pmsSerial.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println("PMS5003: Initialized");
}

int readPM25() {
  if (pms.read(data)) {
    return data.PM_AE_UG_2_5; // คืนค่า PM2.5 หน่วย ug/m3
  }
  return -1; // ถ้ายังไม่มีค่าใหม่ส่งมาให้ส่ง -1
}

#endif