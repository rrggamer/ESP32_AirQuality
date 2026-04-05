#ifndef WEB_SYSTEM_H
#define WEB_SYSTEM_H

#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>
#include <ESPmDNS.h>
#include "credentials.h"
#include "index_html.h"

// อ้างอิงตัวแปรจากไฟล์หลัก
extern AsyncWebServer server;
extern int current_pm25;

// ฟังก์ชันแทนที่ตัวแปรในหน้า HTML ตอนโหลดครั้งแรก
String processor(const String& var){
  if(var == "PM25") {
    return String(current_pm25);
  }
  return String();
}

void setupNetwork() {
  // 1. ตั้งค่า WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // 2. ตั้งค่า mDNS (เข้าผ่าน http://ottersense.local)
  if (MDNS.begin("ottersense")) {
    Serial.println("mDNS responder started: http://ottersense.local");
    MDNS.addService("http", "tcp", 80);
  }

  // 3. กำหนดเส้นทางหน้าเว็บ (Routes)
  
  // หน้าหลัก Dashboard
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // ช่องทาง API สำหรับให้หน้าเว็บดึงเลข PM2.5 ไปอัปเดตเองอัตโนมัติ
  server.on("/api/pm25", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(current_pm25));
  });

  // 4. ตั้งค่าระบบ OTA (อัปเกรดผ่านอากาศ)
  ElegantOTA.begin(&server);
  
  // ปิด AutoReboot เพื่อจัดการจังหวะการเริ่มระบบใหม่ให้เสถียรขึ้น (แก้ปัญหาค้าง 100%)
  ElegantOTA.setAutoReboot(false); 
  
  ElegantOTA.onEnd([](bool success) {
    if (success) {
      Serial.println("OTA Update Successful! Rebooting in 2 seconds...");
      delay(2000); 
      ESP.restart();
    } else {
      Serial.println("OTA Update Failed!");
    }
  });

  // 5. เริ่มการทำงานของ Web Server
  server.begin();
  Serial.println("HTTP Server started");
}

#endif