# Smart Water Tank System

An Arduino-based water level monitoring system with real-time control via web and mobile apps. Uses ultrasonic and TDS sensors for monitoring.


## Overview
This project involves an Arduino-based water level monitoring system that uses an ultrasonic sensor and a TDS sensor to monitor water levels and quality. The system provides real-time data and control through a web app and a mobile app built with React Native.

## Features
- **Real-time Monitoring:** Get real-time data on water levels and quality.
- **Control System:** Turn on/off the water pump based on sensor data.
- **Web App:** Access the monitoring system through a web browser.
- **Mobile App:** Control and monitor the system on the go with a mobile app.

## Components
- **Arduino:** Handles sensor data and controls the pump.
- **Ultrasonic Sensor:** Measures the water level.
- **TDS Sensor:** Monitors the water quality.
- **Buzzer and LED:** Provides alerts and status indication.
- **ESP32 Module:** Connects the system to the internet for remote monitoring.

## Setup

### Arduino
1. Connect the sensors and actuators to the Arduino.
2. Install or Copy necessary libraries. [`Check Here`](https://github.com/Thisal-D/Smart-Water-Tank-System/tree/main/Arduino/libraries)
3. Upload the `main.ino` sketch to the Arduino.

### HTTP Server
1. Installation
    - Ensure you have Python installed. You can download it from Python's official website.
    - Install necessary libraries. [`Check Here`](https://github.com/Thisal-D/Smart-Water-Tank-System/blob/main/HttpServer/requirement.txt)
        - Install Flask using pip:
            ```bash
            pip install Flask
            ```
        - Install Flask Cors using pip:
            ```bash
            pip install flask_cors
            ```
### Web App
1. Open the `index.html` file in a web browser.
2. Use the web interface to monitor and control the water levels.

### Mobile App
1. Clone the `MobileApp` directory.

2. Ensure you have Node.js and npm installed. You can download them from [Node.js official website](https://nodejs.org/).
   
3. Install React Native CLI globally:
    ```bash
    npm install -g react-native-cli
    ```

4. Navigate to the 'MobileApp\ArduinoApp'
    ```bash
    cd 'MobileApp\ArduinoApp'
    ```

5. Runnig the App
    - Setup Android or IOS
        - Android :
            - Connect your Android device to your computer via USB.
            - Enable USB debugging on your Android device (Settings > Developer options > USB debugging).

        - IOS : ```Couldn't Test on IOS :/```
            - Connect your iOS device to your Mac via USB.
            - Open the project in Xcode by navigating to MobileApp/ios and opening the .xcworkspace file.
            - Select your connected device from the device list in Xcode.
            - Click the "Run" button in Xcode to build and install the app on your device.
            - You may need to configure your development team in Xcode to sign the app.
             
6. Run the app 
    ```bash
    npx expo start
    ```

### Contributors

<!-- Contributor 1 -->
<a href="https://github.com/DTD1234567" target="_blank">
    <img src="https://github.com/DTD1234567.png" width="20" height="20" alt="DTD1234567's GitHub profile" />
    DTD1234567
</a>

<!-- Contributor 2 -->
<a href="https://github.com/Loda990" target="_blank">
    <img src="https://github.com/Loda990.png" width="20" height="20" alt="Loda990's GitHub profile" />
    Loda990
</a>    

