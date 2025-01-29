# Http Server

This folder contains the backend components, responsible for handling API requests, managing system state, and interacting with other parts of the project like the Arduino, mobile app, and web app.

## Overview

The server is built using **Flask** and is responsible for:

- Handling system control requests (e.g., turn on/off system, manual pump control).
- Serving system status (e.g., pump status, water level, water quality).
- Handling device communication and status tracking.
- Enabling cross-origin resource sharing (CORS) for communication with the frontend (web and mobile).

## Setup

1. **Install dependencies:**

    Make sure you have `pip` installed. Then, install the required Python libraries by running the following command:

    ```bash
    pip install -r requirements.txt
    ```

2. **Configuration:**

    - Replace the **`SERVER`** (line-7) variable in the `HttpServer.py` file with your server's IP address.
    - The server will by default run on port `5000`, but you can change the port if necessary.

## Dependencies

The server requires the following Python packages:

- Flask
- flask_cors

## Running the Server

To start the server, run the following command:


```bash
python HttpServer.py
```

The server will start listening for incoming requests on the specified IP address and port.