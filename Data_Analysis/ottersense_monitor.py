import matplotlib.pyplot as plt
import matplotlib.animation as animation
import requests
import json
from datetime import datetime

# --- Configuration ---
# Replace with your ESP32 IP or mDNS hostname
TARGET_URL = "http://ottersense.local/api/data"
FETCH_INTERVAL_MS = 2000  # Fetch every 2 seconds
MAX_DATA_POINTS = 30      # Max points to show on graph

# Data storage
x_timestamps = []
y_pm25_values = []

# Setup Matplotlib Figure
fig, ax = plt.subplots(figsize=(10, 5))
plt.subplots_adjust(bottom=0.20)

def animate(i):
    try:
        # Request JSON data from ESP32
        response = requests.get(TARGET_URL, timeout=2)
        
        if response.status_code == 200:
            data = response.json()
            pm25 = data['pm25']
            timestamp = data['time']
            
            # Append new data
            x_timestamps.append(timestamp)
            y_pm25_values.append(pm25)
            
            # Limit data points for scrolling effect
            if len(x_timestamps) > MAX_DATA_POINTS:
                x_timestamps.pop(0)
                y_pm25_values.pop(0)
            
            # Clear and Redraw
            ax.clear()
            ax.plot(x_timestamps, y_pm25_values, marker='o', color='#1a73e8', linewidth=2)
            
            # Styling
            ax.set_title("OtterSense: Live PM2.5 Monitoring", fontsize=14, fontweight='bold')
            ax.set_ylabel("PM2.5 (µg/m³)")
            ax.set_xlabel("Time")
            ax.set_ylim(0, max(80, max(y_pm25_values) + 20)) # Adaptive Y-axis
            ax.grid(True, linestyle='--', alpha=0.5)
            
            # Rotate X-axis labels for readability
            plt.xticks(rotation=45, ha='right')
            
            print(f"Fetched: {timestamp} | PM2.5: {pm25} ug/m3")
            
    except Exception as e:
        print(f"Connection Error: {e}")

# Start Animation
# interval determines how often the graph updates
ani = animation.FuncAnimation(fig, animate, interval=FETCH_INTERVAL_MS, cache_frame_data=False)

plt.show()