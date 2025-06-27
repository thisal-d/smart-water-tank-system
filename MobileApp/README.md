# Mobile App (React Native)

A cross-platform mobile app for remote water tank monitoring & control.

---

## 📱 Features

- View water level and quality in real-time
- Pump control and tank system status
- Automatic data refresh
- Simple and intuitive mobile UI

---

## ⚙️ Setup

1. **Install Node.js:** [Download here](https://nodejs.org/)
2. **Install React Native CLI:**
    ```yaml
    npm install -g react-native-cli
    ```
3. **Install project dependencies:**
    ```
    npm install
    ```
4. **Configure Backend Connection:**
   - In [`App.js`](./ArduinoApp/App.js), set:
     ```js
     const SERVER = "http://<YOUR_SERVER_IP>";
     const PORT_ADDRESS = ':5000';
     ```
   - Ensure your mobile device/emulator is on the same network as the backend server.


## **Run the App:**
- **Android:** `react-native run-android`
- **iOS (Mac):** `react-native run-ios`

---

## 🧩 Technologies

- React Native
- REST API (to Flask backend)

---

## 📸 Screenshot

<img src="../README src/app.png" style="max-height:700px">
