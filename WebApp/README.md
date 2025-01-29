# Web App

This folder contains the source code for the web app. The app provides an interface for users to monitor and control the system remotely through a web browser.

## Screenshot

<img src="../README src/web.png" style="width:800px">

## Overview

The web app is responsible for:

- Displaying data from the system, such as water level and water quality.
- Sending control commands to the backend server (e.g., turning on/off the water pump, adjusting settings).
- Interacting with the backend server via Wi-Fi to update and retrieve data.

The web app is built using [**HTML**](./index.html), [**CSS**](style.css), and [**JavaScript**](./script.js).

## Features
- **Water Level Monitoring:** Displays real-time water level data from sensors in the tank.
- **Water Quality Monitoring:** Shows water quality information, such as TDS (Total Dissolved Solids), based on sensor readings.
- **Automatic Data Updates:** Data is updated automatically at regular intervals without requiring user intervention.
- **User-Friendly Interface:** Simple and easy-to-use design for viewing and controlling the system.

## Setup

### 1. Configuring the Server IP

To configure the server IP for the web app, you need to modify the connection settings in the JavaScript file (`script.js`).

1. Open the [`script.js`](./script.js) file in your project.
2. Update the `SERVER` constant with the IP address of the [Flask server](../HttpServer/):

    ```javascript
    const SERVER = "http://xxx.xxx.xxx.xxx";  // Replace with your server IP address
    ```

3. The `PORT_ADDRESS` should also be configured to match the port the Flask server is running on:

    ```javascript
    const PORT_ADDRESS = ':5000';  // Default Flask port
    ```

4. Make sure your server and the web app are on the same Wi-Fi network to allow proper communication.

After making these changes, the web app will be able to communicate with the server using the provided IP address and port.

### 3. Running the Web App

To run the app locally:

1. Open the [`index.html`](./index.html) file in your preferred browser.
2. The web app will be displayed, and it will automatically connect to the Flask server for data.

### 4. Testing the App

Once the app is running, you can interact with it by:

- Viewing system data such as water levels and water quality.
- Sending commands to control the system, such as turning the pump on or off.
- Ensuring that data updates automatically based on the server’s response.
