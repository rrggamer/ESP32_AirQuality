#ifndef INDEX_HTML_H
#define INDEX_HTML_H

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>OtterSense Dashboard</title>
  <style>
    body { font-family: sans-serif; text-align: center; background-color: #f0f2f5; margin: 0; padding: 20px; }
    .card { background: white; padding: 30px; border-radius: 20px; box-shadow: 0 10px 25px rgba(0,0,0,0.1); max-width: 400px; margin: auto; }
    h1 { color: #1a73e8; }
    .value { font-size: 4rem; color: #202124; font-weight: bold; }
    .unit { color: #70757a; }
    .version { font-size: 0.7rem; color: #ccc; margin-top: 20px; }
  </style>
</head>
<body>
  <div class="card">
    <h1>🦦 OtterSense S3</h1>
    <div class="value" id="pm25">%PM25%</div>
    <div class="unit">µg/m³ (PM2.5)</div>
    <div class="version">v1.3 | Auto-Refresh Mode</div>
  </div>

  <script>
    setInterval(function() {
      // แอบไปดึงข้อมูลจากเส้นทาง /api/pm25 ทุกๆ 2 วินาที
      fetch('/api/pm25')
        .then(response => response.text())
        .then(data => {
          // เอาตัวเลขใหม่มาเสียบแทนที่เดิม
          document.getElementById("pm25").innerText = data;
        });
    }, 2000); // 2000 มิลลิวินาที = 2 วินาที
  </script>
</body>
</html>)rawliteral";

#endif