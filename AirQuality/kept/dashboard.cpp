#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>
#include "credentials.h"
#include <ESPmDNS.h>

AsyncWebServer server(80);

// --- หน้าตาของ Web Dashboard (HTML/CSS) ---
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>OtterSense Dashboard</title>
  <style>
    body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; text-align: center; background-color: #f0f2f5; margin: 0; padding: 20px; }
    .container { max-width: 400px; margin: auto; }
    .card { background: white; padding: 30px; border-radius: 20px; box-shadow: 0 10px 25px rgba(0,0,0,0.1); transition: 0.3s; }
    .card:hover { transform: translateY(-5px); }
    h1 { color: #1a73e8; margin-bottom: 10px; font-size: 1.8rem; }
    p { color: #5f6368; margin-bottom: 30px; }
    .status-badge { background: #e6f4ea; color: #1e8e3e; padding: 5px 15px; border-radius: 20px; font-size: 0.8rem; font-weight: bold; }
    .value-container { margin: 20px 0; }
    .value { font-size: 4rem; color: #202124; font-weight: 800; line-height: 1; }
    .unit { font-size: 1.1rem; color: #70757a; margin-top: 5px; }
    .footer { margin-top: 30px; font-size: 0.8rem; color: #9aa0a6; }
  </style>
</head>
<body>
  <div class="container">
    <div class="card">
      <span class="status-badge">● ONLINE</span>
      <h1>🦦 OtterSense S3</h1>
      <p>ระบบตรวจวัดคุณภาพอากาศ</p>
      
      <div class="value-container">
        <div class="value">%PM25%</div>
        <div class="unit">µg/m³ (PM2.5)</div>
      </div>

      <div class="footer">
        Device: ESP32-S3 | Location: Pa Daet
      </div>
    </div>
  </div>
</body>
</html>)rawliteral";

// ฟังก์ชันสำหรับส่งค่าจากตัวแปรใน C++ ไปแสดงในหน้า HTML
String processor(const String& var){
  if(var == "PM25"){  
    return String(555550); 
  }
  return String();
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // 1. เชื่อมต่อ WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }

  // 2. ตั้งค่า mDNS
  if (MDNS.begin("ottersense")) {
      Serial.println("mDNS started: http://ottersense.local");
      MDNS.addService("http", "tcp", 80);
  }

  // 3. กำหนดเส้นทาง (Routes) ของหน้าเว็บ
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    // ส่งหน้า HTML ออกไปพร้อมใช้ processor แทนที่ตัวแปร
    request->send_P(200, "text/html", index_html, processor);
  });

  // 4. เริ่มระบบ OTA และ WebServer
  ElegantOTA.begin(&server);

  ElegantOTA.setAutoReboot(false);

  ElegantOTA.onEnd([](bool success) {
    if (success) {
      Serial.println("OTA Update Finished Successfully! Rebooting...");
      delay(3000);
      Serial.println("Rebooting Now!");
      ESP.restart();
    }

  });

  server.begin();
  
  Serial.println("\n--- Dashboard Ready ---");
  Serial.print("Access via: http://");
  Serial.println(WiFi.localIP());
}

void loop() {
  // หน้าเว็บแบบนี้ยังไม่ต้องเขียนอะไรใน loop ครับ
}