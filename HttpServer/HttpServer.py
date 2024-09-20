from flask import Flask, request
from flask_cors import CORS
import time
import threading


SERVER = '192.168.17.24'
PORT = 5000

app = Flask("")
CORS(app)  # Enable CORS for all routes

# System Initial status
system_state = True
buzzer_status = False
pump_status = False
led_green_status = False
led_red_status = False
pump_control_method = "automatic"
pump_manual_controlled_status = False 
water_level = 0
water_mgl_value = 0
device_status = 0

last_online_time = time.time() # get current time

def track_device_status():
    global device_status
    while True:
        delay = time.time() - last_online_time
        if delay > 5:
            device_status = False
        time.sleep(5)
        #print(device_status)

@app.route('/get-device-status', methods=['GET'])
def get_device_status():
    return {
        "device_status": device_status
    }


@app.route('/get-pump_control_method', methods=['GET'])
def get_pump_control_method():
    return {
        "pump_control_method": pump_control_method,
    }
    

@app.route('/get-pump-manual-controlled-status', methods=['GET'])
def get_manual_controlled_pump_Status():
    return {
        'pump_manual_controlled_status' : pump_manual_controlled_status,
    }
       
                
@app.route('/get-system-status', methods=['GET'])
def get_system_status():
    global device_status, last_online_time
    last_online_time = time.time() # get current time
    device_status = True
    return {"system_status": system_state }
    

@app.route('/get-status', methods=['GET'])
def get_status():
    return {
        'system_status': system_state,
        'buzzer_status': buzzer_status,
        'pump_status': pump_status,
        'led_green_status': led_green_status,
        'led_red_status': led_red_status,
        'water_level': water_level,
        'water_mgl_value': water_mgl_value,
        'pump_control_method': pump_control_method,
        'pump_manual_controlled_status': pump_manual_controlled_status,
        'device_status' : device_status
    }

@app.route('/send-status', methods=['POST'])
def send_status():
    global buzzer_status, pump_status, led_green_status, led_red_status, water_level, water_mgl_value

    data = request.get_json()
    buzzer_status = True if data['buzzer_status'] == "1" else False
    pump_status = True if data['pump_status'] == "1" else False
    led_green_status = True if data['led_green_status'] == "1" else False
    led_red_status = True if data['led_red_status'] == "1" else False
    water_level = data['water_level']
    water_mgl_value = data['water_mgl_value']
    
    return '', 204

@app.route('/send-buzzer-status', methods=['POST'])
def send_buzzer_status():
    global buzzer_status
    
    data = request.get_json()
    buzzer_status = True if data['buzzer_status'] == "1" else False
    return '', 204

@app.route('/turn-on-pump-manually', methods=['POST'])
def turn_on_pump_manually():
    global pump_control_method, pump_manual_controlled_status
    pump_control_method = "manual"
    pump_manual_controlled_status = True
    return '', 204

@app.route('/turn-off-pump-manually', methods=['POST'])
def turn_off_pump_manually():
    global pump_control_method, pump_manual_controlled_status
    pump_control_method = "manual"
    pump_manual_controlled_status = False
    return '', 204

@app.route('/turn-off-system', methods=['POST'])
def turn_off_system():
    global system_state
    system_state = False
    return '', 204

@app.route('/turn-on-system', methods=['POST'])
def turn_on_system():
    global system_state
    system_state = True
    return '', 204

@app.route('/turn-on-automatic-pump-control-mode', methods=['POST'])
def turn_off_manual_pump_activation():
    global pump_control_method
    pump_control_method = "automatic"
    return '', 204

device_status_track_thread = threading.Thread(target=track_device_status, daemon=True)
device_status_track_thread.start()

app.run(debug=True, host=SERVER, port=PORT)
