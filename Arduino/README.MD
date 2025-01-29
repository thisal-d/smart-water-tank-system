# Arduino Code

This folder contains the Arduino code for [Your Project Name]. The Arduino sketches handle the system's hardware interaction, such as sensors, actuators, and communication with the backend (server).

## Overview

The Arduino code is responsible for:

- Reading sensor data (e.g., water level, TDS sensor).
- Controlling actuators (e.g., water pump, LEDs, buzzer).
- Communicating with the backend server via [choose method: Serial, Wi-Fi, Bluetooth, etc.].
- Managing system logic based on sensor inputs and control commands.


## Setup

### 1. Install Arduino IDE

Make sure you have the **Arduino IDE** installed. You can download it from [here](https://www.arduino.cc/en/software).

### 2. Select Board and Port

- Open the Arduino IDE.
- Go to **Tools** → **Board** and select the correct board (e.g., **ESP32**).
- Go to **Tools** → **Port** and select the correct port.

### 3. Install Libraries

To use the required libraries, you need to install them via the Arduino Library Manager:

1. **ArduinoJson**: Used for parsing and generating JSON data.  
   - Go to **Sketch** → **Include Library** → **Manage Libraries**.
   - Search for **ArduinoJson** and install it.

2. **LiquidCrystal_I2C**: Used to interface with an I2C-based LCD.  
   - Go to **Sketch** → **Include Library** → **Manage Libraries**.
   - Search for **LiquidCrystal_I2C** and install it.

### 4. [Configure Pins](./pin_confuguration.md)

Ensure the communication between the Arduino and the server is correctly set up (e.g., Serial, Wi-Fi). Check the code for the connection method and make any necessary adjustments (e.g., network credentials, IP address).

- **Output Pins**  
  - **Water Pump Relay:** `GPIO 14`  
  - **Red LED:** `GPIO 13`  
  - **Green LED:** `GPIO 12`  
  - **Buzzer:** `GPIO 25`  

- **Input/Output Pins**  
  - **Ultrasonic Sensor**  
    - **Echo (Input):** `GPIO 27`  
    - **Trigger (Output):** `GPIO 26`  

- **Input Pins**  
  - **TDS Sensor:** `GPIO 35`  


### 5. Configuring the Server IP

To configure the server IP, follow these steps to modify the connection settings in the [`sketch_oct6a.ino`](./sketch_oct6a/sketch_oct6a.ino) file:

1. Open the [`sketch_oct6a.ino`](./sketch_oct6a/sketch_oct6a.ino) file in your project.
2. Locate the `serverUrl` variable and update it with the IP address of your [Flask server](../HttpServer/). Example:

    ```cpp
    const String serverUrl = "http://xxx.xxx.xxx.xxx:5000";  // Replace with your server IP address
    ```

    - Replace `xxx.xxx.xxx.xxx` with your server's IP address.
    - The default port for the Flask server is `5000`. Ensure that your Flask server is running on this port, or modify it if necessary.

3. Ensure that your server and the Arduino board are connected to the **same Wi-Fi network** to enable proper communication between them.

### 6. Configure SSID & Password

To connect your Arduino to a Wi-Fi network, you need to set the **SSID** (network name) and **password** in the [`sketch_oct6a.ino`](./sketch_oct6a/sketch_oct6a.ino) file:

1. Open the [`sketch_oct6a.ino`](./sketch_oct6a/sketch_oct6a.ino) file.
2. Find the lines where the Wi-Fi credentials are set:

    ```cpp
    const char* ssid = "YOUR_SSID";      // Replace with your Wi-Fi network name
    const char* password = "YOUR_PASSWORD"; // Replace with your Wi-Fi network password
    ```

3. Replace `YOUR_SSID` with the name of your Wi-Fi network and `YOUR_PASSWORD` with your network’s password.


### 7. Upload the Code

- Open the `sketch_oct6a.ino` file in the Arduino IDE.
- Ensure that the `Functions.ino` file is also included in the project folder as it contains all the functions used in the main code.
- Click the **Upload** button to upload the code to your Arduino board.



## Code Explanation

The Arduino sketch consists of two files:

- **[sketch_oct6a.ino](./sketch_oct6a/sketch_oct6a.ino)**: The main Arduino sketch file, where the setup and loop functions are defined.
- **[Functions.ino](./sketch_oct6a/Functions.ino)**: Contains all the functions that handle sensor readings, actuator control, and communication with the backend server.

### Key Functionalities:

- **Sensor Reading**: Code for reading sensor values from the TDS sensor and ultrasonic sensor.
- **Actuator Control**: Code to turn on/off the water pump, LEDs, and buzzer based on system requirements.
- **Communication with Server**: Code to send sensor data and receive control commands from the backend server.

## Dependencies

- **ArduinoJson**: A library used for parsing and generating JSON data.
- **LiquidCrystal_I2C**: A library used for interfacing with an I2C LCD.

You can install these libraries through the Arduino Library Manager or download them from the official repositories.
