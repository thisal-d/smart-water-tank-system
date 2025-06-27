# Smart Water Tank System

A robust and extensible IoT platform for real-time water level and quality monitoring using Arduino/ESP32, TDS/ultrasonic sensors, and accessible via web and mobile applications.

## 💡 Project Overview

This project enables smart monitoring and control of a water tank. It uses sensors to collect water level and quality data, and provides user interfaces for monitoring and management through a web app and a cross-platform mobile app.

**Main Technologies:**  
- Hardware: ESP32, Ultrasonic Sensor, TDS Sensor  
- Backend: Python (Flask)  
- Web: HTML, CSS, JavaScript  
- Mobile: React Native

---

## 🚀 Features

- **Automated Pump Control:** Turns the pump on/off based on water level and quality.
- **Sensor Monitoring:** Real-time water level and TDS data.
- **Alerts/Notifications:** Audible and visual alerts for critical tank conditions.
- **User Interfaces:** 
  - Web dashboard
  - Mobile app for Android/iOS
- **Remote Access:** Control and monitor from anywhere on the local network.

---

## 🗂️ Project Structure

Smart-Water-Tank-System/ <br>
│ <br>
├── [**Arduino/**](./Arduino/) # Embedded code for ESP32 and sensors <br>
│ <br>
├── [**HttpServer/**](./HttpServer/) # Flask backend server for REST API & logic <br> 
│ <br>
├── [**WebApp/**](./WebApp/) # Web dashboard for control & monitoring <br>
│ <br>
├── [**MobileApp/**](./MobileApp/) # React Native mobile app for remote access <br>
│ <br>
└── **README.md** # Project overview and documentation



---

## 🔗 Quick Links

- [Arduino Firmware](./Arduino/)
- [Backend Server (Flask)](./HttpServer/)
- [Web Dashboard](./WebApp/)
- [Mobile App](./MobileApp/)

---

## 🖼️ Sample Interfaces

| Mobile App | Web Dashboard | Server Console |
|---|---|---|
| ![Mobile App](./README%20src/app.png) | ![Web App](./README%20src/web.png) | ![Server](./README%20src/server.png) |

---

## 🛠️ Getting Started

Each component has its own README for detailed setup. Start with the backend, then configure the hardware, and finally set up your preferred frontend.

---

## 📄 License

This project is MIT licensed.