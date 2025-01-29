# Smart Water Tank System

An Arduino-based water level and quality monitoring system with control via web and mobile apps. Uses ultrasonic and TDS sensors to measure water level and quality.

## Overview
This project features an **Arduino-based smart water tank system** that monitors water levels and quality using an **ultrasonic sensor** and a **TDS sensor**. The system provides data and control via a **web app** and a **mobile app** built with React Native.

## Features
- **Automated Control:** Automatically turns the water pump on/off based on sensor data.
- **Web Interface:** Monitor and control the system via a web browser.
- **Mobile App:** Access monitoring and control features on the go.
- **Alerts & Notifications:** Uses a buzzer and LEDs for system status indications.

## Components
- **ESP32:** Microcontroller handling sensor data and pump control.
- **Ultrasonic Sensor:** Measures water level.
- **TDS Sensor:** Monitors water quality.
- **Buzzer:** Provides alerts for critical water conditions.
- **LED Indicators:**
  - **Red:** System status indication.
  - **Green:** Water pump status indication.
- **Internet Connectivity:** Enables remote monitoring and control via web and mobile apps.


## Project Structure
**Smart-Water-Tank-System**<br>
&nbsp;&nbsp;&nbsp;&nbsp;│<br>
&nbsp;&nbsp;&nbsp;&nbsp;├── [_`Arduino/`_](./Arduino/)<br>
&nbsp;&nbsp;&nbsp;&nbsp;│    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;└──  [_`sketch_oct6a`_](./Arduino/sketch_oct6a/)<br>
&nbsp;&nbsp;&nbsp;&nbsp;│    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;     &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├── [`sketch_oct6a.ino`](./Arduino/sketch_oct6a/sketch_oct6a.ino)<br>
&nbsp;&nbsp;&nbsp;&nbsp;│    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;     &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;└── [`Functions.ino`](./Arduino/sketch_oct6a/Functions.ino)<br>
&nbsp;&nbsp;&nbsp;&nbsp;│<br>
&nbsp;&nbsp;&nbsp;&nbsp;├── [_`HttpServer/`_](./HttpServer/)<br>
&nbsp;&nbsp;&nbsp;&nbsp;│    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;└──[`HttpServer.py/`](./HttpServer/HttpServer.py)<br>
&nbsp;&nbsp;&nbsp;&nbsp;│<br>
&nbsp;&nbsp;&nbsp;&nbsp;├── [_`MobileApp/`_](./MobileApp/)<br>
&nbsp;&nbsp;&nbsp;&nbsp;│<br>
&nbsp;&nbsp;&nbsp;&nbsp;└── [_`WebApp/`_](./WebApp/)<br>

---

## Setup Guide

### 1. [Arduino Setup](./Arduino/)
1. Connect the sensors and actuators to the Arduino.
2. Install the required libraries from [`libraries.txt`](./Arduino/sketch_oct6a/libraries.txt).
3. Upload the [`sketch_oct6a.ino`](./Arduino/sketch_oct6a/sketch_oct6a.ino) sketch to the Arduino.

### 2. [HTTP Server Setup](./HttpServer/)
1. Ensure **Python** is installed ([Download Python](https://www.python.org/)).
2. Install dependencies from [`requirement.txt`](./HttpServer/requirement.txt):
 
    ```bash
    pip install -r requirements.txt
    ```

### 3. [Web App Setup](./WebApp/)
1. Open the `index.html` file in a web browser.
2. Use the interface to monitor and control the water levels remotely.

### 4. [Mobile App Setup](./MobileApp/)
1. Clone the `MobileApp` directory.
2. Install **Node.js** and **npm** ([Download Node.js](https://nodejs.org/)).
3. Install React Native CLI globally:
    ```bash
    npm install -g react-native-cli
    ```
4. Navigate to the mobile app directory:
    ```bash
    cd MobileApp/ArduinoApp
    ```
5. Running the App:
   - **Android:**
     - Connect your Android device via USB.
     - Enable **USB debugging** (Settings > Developer options > USB debugging).
   - **iOS:** *(Not tested)*
     - Open the project in Xcode (`MobileApp/ios` > `.xcworkspace`).
     - Select your connected device and click **Run**.
6. Start the app:
 
    ```bash
    npx expo start
    ```

---

## Contributors

### Main Contributors:

- [<img src="https://github.com/DTD1234567.png" width="20" height="20" alt="DTD1234567's GitHub profile" /> DTD1234567](https://github.com/DTD1234567)
- [<img src="https://github.com/Loda990.png" width="20" height="20" alt="Loda990's GitHub profile" /> Loda990](https://github.com/Loda990)

---

### License
This project is open-source and available under the **MIT License**.

---

### Feedback & Support
For any issues or improvements, feel free to open an **issue** or submit a **pull request** on GitHub!
