#ifndef INDEX_HTML_H
#define INDEX_HTML_H

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>OtterSense Pro</title>
  <style>
    body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; text-align: center; background-color: #f4f7f9; margin: 0; padding: 20px; color: #333; }
    .container { max-width: 1000px; margin: auto; display: grid; grid-template-columns: repeat(auto-fit, minmax(280px, 1fr)); gap: 20px; }
    .header { grid-column: 1 / -1; margin-bottom: 20px; }
    .card { background: white; padding: 25px; border-radius: 15px; box-shadow: 0 4px 15px rgba(0,0,0,0.05); transition: transform 0.2s; }
    .card:hover { transform: translateY(-5px); }
    .label { font-size: 0.9rem; color: #666; font-weight: bold; text-transform: uppercase; }
    .value { font-size: 2.5rem; margin: 10px 0; font-weight: 600; color: #1a73e8; }
    .unit { font-size: 1rem; color: #888; }
    .status { font-size: 0.8rem; color: #999; grid-column: 1 / -1; margin-top: 20px; }
    .pm25-card { border-top: 5px solid #1a73e8; }
    .env-card { border-top: 5px solid #34a853; }
    .gas-card { border-top: 5px solid #fbbc05; }
  </style>
</head>
<body>
  <div class="header">
    <h1>OtterSense Pro Dashboard</h1>
    <p>Real-time Air Quality & Environment Monitoring</p>
  </div>

  <div class="container">
    <div class="card pm25-card">
      <div class="label">Particulate Matter</div>
      <div class="value" id="pm25">--</div>
      <div class="unit">µg/m³ (PM2.5)</div>
    </div>

    <div class="card env-card">
      <div class="label">Temperature</div>
      <div class="value" id="temp">--</div>
      <div class="unit">°C (AHT21)</div>
    </div>
    <div class="card env-card">
      <div class="label">Humidity</div>
      <div class="value" id="hum">--</div>
      <div class="unit">% (AHT21)</div>
    </div>
    <div class="card env-card">
      <div class="label">Air Pressure</div>
      <div class="value" id="pres">--</div>
      <div class="unit">hPa (BMP280)</div>
    </div>

    <div class="card gas-card">
      <div class="label">eCO2</div>
      <div class="value" id="eco2">--</div>
      <div class="unit">ppm</div>
    </div>
    <div class="card gas-card">
      <div class="label">TVOC</div>
      <div class="value" id="tvoc">--</div>
      <div class="unit">ppb</div>
    </div>
  </div>

  <div id="timestamp" class="status">Connecting to OtterSense station...</div>

  <script>
    function updateData() {
      fetch('/api/data')
        .then(response => response.json())
        .then(data => {
          document.getElementById("pm25").innerText = data.pm25;
          document.getElementById("temp").innerText = data.temp.toFixed(1);
          document.getElementById("hum").innerText = data.hum.toFixed(1);
          document.getElementById("pres").innerText = data.pres.toFixed(1);
          document.getElementById("eco2").innerText = data.eco2;
          document.getElementById("tvoc").innerText = data.tvoc;
          document.getElementById("timestamp").innerText = "Last Sync: " + data.time;
        })
        .catch(err => {
          document.getElementById("timestamp").innerText = "Connection lost. Reconnecting...";
        });
    }
    setInterval(updateData, 2000);
    updateData();
  </script>
</body>
</html>)rawliteral";

#endif