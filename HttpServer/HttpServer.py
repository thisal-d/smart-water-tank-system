from flask import Flask, request
from flask_cors import CORS
import time
import threading

# Server Configuration
SERVER = "xxx.xxx.xxx.xxx"  # Replace with your IP address
PORT = 5000

app = Flask("")
CORS(app)  # Enable CORS for all routes

# System Initial status
system_state = True
buzzer_status = False
pump_status = True
led_green_status = False
led_red_status = False
pump_control_method = "automatic"
pump_manual_controlled_status = False 
water_level = 0
water_level_rate = 0
water_quality = "poor"
device_status = False
tank_height = 0

last_online_time = time.time()  # Get current time

def print_status():
    """
    Prints the current status of the system including water level, pump status, 
    buzzer status, LED status, and more.
    """
    print("system_state :", system_state)
    print("buzzer_status :", buzzer_status)
    print("pump_status :", pump_status)
    print("led_green_status :", led_green_status)
    print("led_red_status :", led_red_status)
    print("pump_control_method :", pump_control_method)
    print("pump_manual_controlled_status :", pump_manual_controlled_status)
    print("water_level :", water_level)
    print("tank_height:", tank_height)
    print("water_level_rate :", water_level_rate)
    print("water_quality :", water_quality)
    print("device_status :", device_status)

def track_device_status():
    """
    Tracks the device status by checking the time elapsed since the last 
    online update. If the device is offline for more than 5 seconds, 
    it updates the device status to False.
    """
    global device_status
    while True:
        delay = time.time() - last_online_time
        if delay > 5:
            device_status = False
        time.sleep(5)

@app.route("/get-system-control-status", methods=["GET"])
def get_system_control_status():
    """
    Endpoint to retrieve the current system control status including system state, 
    pump control method, and manual pump control status.
    """
    return {
        "system_status": system_state,
        "pump_control_method": pump_control_method,
        "pump_manual_controlled_status": pump_manual_controlled_status
    }

@app.route("/get-status", methods=["GET"])
def get_status():
    """
    Endpoint to retrieve the current system status including buzzer status, pump 
    status, LED status, water level, water quality, and more.
    """
    print_status()
    return {
        "system_status": system_state,
        "buzzer_status": buzzer_status,
        "pump_status": pump_status,
        "led_green_status": led_green_status,
        "led_red_status": led_red_status,
        "water_level": water_level,
        "water_level_rate": water_level_rate,
        "tank_height": tank_height,
        "water_quality": water_quality,
        "pump_control_method": pump_control_method,
        "pump_manual_controlled_status": pump_manual_controlled_status,
        "device_status": device_status
    }

@app.route("/send-status", methods=["POST"])
def send_status():
    """
    Endpoint to receive and update the system's status including buzzer, pump, 
    LED, water level, tank height, water level rate, and water quality.
    Also updates the last online time and device status.
    """
    global buzzer_status, pump_status, led_green_status, led_red_status, water_level, water_level_rate, water_quality, tank_height
    global device_status, last_online_time
    last_online_time = time.time()  # Get current time
    
    device_status = True
    data = request.get_json()
    buzzer_status = True if data["buzzer_status"] == "1" else False
    pump_status = True if data["pump_status"] == "1" else False
    led_green_status = True if data["led_green_status"] == "1" else False
    led_red_status = True if data["led_red_status"] == "1" else False
    water_level = int(data["water_level"])
    tank_height = int(data["tank_height"])
    water_level_rate = float(data["water_level_rate"])
    water_quality = data["water_quality"]
    print_status()
    
    return "", 204

@app.route("/send-buzzer-status", methods=["POST"])
def send_buzzer_status():
    """
    Endpoint to update the buzzer status. 
    Accepts a POST request to toggle the buzzer on or off.
    """
    global buzzer_status
    
    data = request.get_json()
    buzzer_status = True if data["buzzer_status"] == "1" else False
    return "", 204

@app.route("/turn-on-pump-manually", methods=["POST"])
def turn_on_pump_manually():
    """
    Endpoint to manually turn on the pump by changing the control mode to "manual".
    """
    global pump_control_method, pump_manual_controlled_status
    pump_control_method = "manual"
    pump_manual_controlled_status = True
    return "", 204

@app.route("/turn-off-pump-manually", methods=["POST"])
def turn_off_pump_manually():
    """
    Endpoint to manually turn off the pump by changing the control mode to "manual".
    """
    global pump_control_method, pump_manual_controlled_status
    pump_control_method = "manual"
    pump_manual_controlled_status = False
    return "", 204

@app.route("/turn-off-system", methods=["POST"])
def turn_off_system():
    """
    Endpoint to turn off the system by setting system state to False.
    """
    global system_state
    system_state = False
    return "", 204

@app.route("/turn-on-system", methods=["POST"])
def turn_on_system():
    """
    Endpoint to turn on the system by setting system state to True.
    """
    global system_state
    system_state = True
    return "", 204

@app.route("/turn-on-automatic-pump-control-mode", methods=["POST"])
def turn_off_manual_pump_activation():
    """
    Endpoint to switch the pump control mode back to "automatic".
    """
    global pump_control_method
    pump_control_method = "automatic"
    return "", 204

# Run device status tracker as different thread
device_status_track_thread = threading.Thread(target=track_device_status, daemon=True)
device_status_track_thread.start()

# Start the Flask server
app.run(debug=True, host=SERVER, port=PORT)
