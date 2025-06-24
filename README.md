# Smart-Waste-Management-System

This is an IoT-based system that monitors waste levels and sends alerts using ESP32 and Blynk IoT platform.
This project measures the fill level of a dustbin in real-time and sends alerts when it reaches critical thresholds, enabling efficient waste collection.

---

# Features

- Real-time monitoring of bin fill level
- Visual representation via Blynk Gauge widget
- Alerts when the bin is half-full and almost full
- Wi-Fi enabled with ESP32
- Customizable thresholds

---

# Hardware Used

| Component            | Quantity |
|----------------------|----------|
| ESP32 Dev Board      | 1        |
| Ultrasonic Sensor (HC-SR04) | 1        |
| Jumper Wires         | As needed |
| USB Cable            | 1        |
| Wi-Fi (2.4GHz)       | Required |

---

# Blynk Setup

1. **Create an account** at [https://blynk.io](https://blynk.io)
2. **Create a new Template**
   - Template Name: `IOT Waste Management`
   - Hardware: `ESP32`
3. **Create Data Stream**
   - Name: `Fill Level`
   - Virtual Pin: `V1`
   - Type: `Integer`
   - Min: `0`, Max: `100`, Unit: `%`
4. **Add Widgets**
   - **Gauge Widget**
     - Data Stream: `V1`
     - Label: `Fill Level`
     - Range: `0–100%`
   - **Event Notifications**
     - Add events for:
       - `half_full`: "Bin is half full"
       - `bucket_full`: "Bin is almost full"

---

# How It Works

- The ultrasonic sensor measures the distance from the bin lid to the trash level.
- The code calculates the **fill percentage** based on calibrated "empty" and "full" distances.
- It pushes real-time values to the Blynk cloud.
- When the bin is 50% or 95% full, alerts are triggered using `Blynk.logEvent`.

---

# Calibration

Update the following constants in `sketch.ino` based on your bin's dimensions:

```cpp
const int EMPTY_BIN_DISTANCE = 100;  // Distance when bin is empty
const int FULL_BIN_DISTANCE = 5;    // Distance when bin is full
