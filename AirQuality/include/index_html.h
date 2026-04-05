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
    .status { font-size: 0.8rem; color: #999; margin-top: 20px; }
  </style>
</head>
<body>
  <div class="card">
    <h1>OtterSense Dashboard</h1>
    <div class="value" id="pm25">%PM25%</div>
    <div class="unit">µg/m³ (PM2.5)</div>
    <div id="timestamp" class="status">Loading data...</div>
  </div>

  <script>
    // Fetch data from API every 2 seconds
    setInterval(function() {
      fetch('/api/data')
        .then(response => {
          if (!response.ok) throw new Error('Network response was not ok');
          return response.json();
        })
        .then(data => {
          // Update UI with new values
          document.getElementById("pm25").innerText = data.pm25;
          document.getElementById("timestamp").innerText = "Last Updated: " + data.time;
        })
        .catch(error => {
          console.error('Fetch error:', error);
          document.getElementById("timestamp").innerText = "Connection Lost";
        });
    }, 2000);
  </script>
</body>
</html>)rawliteral";

#endif