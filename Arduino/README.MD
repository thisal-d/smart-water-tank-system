# Arduino Firmware for Smart Water Tank System

This directory contains the embedded code for the ESP32 microcontroller, managing all sensor readings, pump/alert control, and communication with the backend server.

---

## ✨ Responsibilities

- Read water level (Ultrasonic) and TDS (Total Dissolved Solids) sensors
- Control Relay (water pump), LEDs, and buzzer based on logic
- Communicate with backend server (HTTP over Wi-Fi)
- Respond to commands from web/mobile apps

---

## ⚡ Hardware Pinout

| Function            | Pin         |
|---------------------|-------------|
| Water Pump Relay    | GPIO 14     |
| Red LED             | GPIO 13     |
| Green LED           | GPIO 12     |
| Buzzer              | GPIO 25     |
| Ultrasonic Trigger  | GPIO 26     |
| Ultrasonic Echo     | GPIO 27     |
| TDS Sensor          | GPIO 35     |

---

## 🛠️ Setup Instructions

1. **Install Arduino IDE**: [Download here](https://www.arduino.cc/en/software)
2. **Select Board:** ESP32 (via Tools → Board)
3. **Install Required Libraries:**
   - `ArduinoJson`
   - `LiquidCrystal_I2C`
4. **Configure Network:**
   - Edit `sketch_oct6a.ino` to set your Wi-Fi credentials and server IP:
     ```cpp
     const String serverUrl = "http://<YOUR_SERVER_IP>:5000";
     ```
5. **Upload Firmware:** Use the IDE to upload to your ESP32.

---

## 📎 Additional Notes

- Ensure the ESP32 and backend server are on the same local network.
- Adjust pin assignments in code if your hardware layout differs.

For further details, see [`sketch_oct6a.ino`](./sketch_oct6a/sketch_oct6a.ino).