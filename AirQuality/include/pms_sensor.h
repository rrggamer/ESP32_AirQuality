#ifndef PMS_SENSOR_H
#define PMS_SENSOR_H

#include <Arduino.h>
#include "PMS.h"

HardwareSerial pmsSerial(2);
PMS pms(pmsSerial);
PMS::DATA data;

void setupPMS() {
  //RX16 TX17
  pmsSerial.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println("PMS5003: Initialized");
}

int readPM25() {
  if (pms.read(data)) {
    return data.PM_AE_UG_2_5; 
  }
  return -1; 
}

#endif