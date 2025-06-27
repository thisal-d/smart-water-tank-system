# Backend Server (Flask)

This folder contains the backend API server, acting as the bridge between the hardware (ESP32) and the user interfaces (web & mobile).

---

## 🧠 Responsibilities

- Receive and process sensor data from ESP32
- Provide RESTful API for frontend apps
- Manage tank state and pump/alert logic
- Enable CORS for cross-origin frontend access

---

## ⚙️ Setup

1. **Install Python 3 & pip**
2. **Install dependencies:**
    ```yaml
    pip install -r requirements.txt
    ```
3. **Configure Server IP:**
   - Edit `HttpServer.py` and replace the `SERVER` variable with your IP (default port: 5000)
4. **Run the server:**
   
    ```yaml
    python HttpServer.py
    ```

The server will listen for requests at `http://<YOUR_SERVER_IP>:5000`.

---

## 🧩 Dependencies

- Flask
- flask_cors

---

## 📝 API Reference

> Refer to code comments in `HttpServer.py` for endpoint documentation.

---

## 📸 Screenshot


<img src="../README src/server.png" style="width:800px">
