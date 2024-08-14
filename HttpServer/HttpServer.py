from flask import Flask, jsonify, request
from flask_cors import CORS

app = Flask("")
CORS(app)  # Enable CORS for all routes

# Initial states
system_state = False
buzzer_status = "OFF"
pump_status = "OFF"
led_status = "OFF"
distance_to_water = "0"
water_ph_value = "0"

@app.route('/status', methods=['GET'])
def status():
    return {
        'system_status': "ON" if system_state else "OFF",
        'buzzer_status': buzzer_status,
        'pump_status': pump_status,
        'led_status': led_status,
        'distance_to_water': distance_to_water,
        'water_ph_value': water_ph_value
    }
    
@app.route('/get_system', methods=['GET'])
def get_system():
    return {
        'system_status': system_state,
    }
    
@app.route('/toggle', methods=['POST'])
def toggle():
    global system_state
    system_state = not system_state 
    return '', 204
    

@app.route('/update', methods=['POST'])
def update():
    global buzzer_status, pump_status, led_status, distance_to_water, water_ph_value
    data = request.get_json()
    buzzer_status = data.get('buzzer_status', buzzer_status)
    pump_status = data.get('pump_status', pump_status)
    led_status = data.get('led_status', led_status)
    distance_to_water = data.get('distance_to_water', distance_to_water)
    water_ph_value = data.get('water_ph_value', water_ph_value)
    return '', 204


app.run(debug=True, host='192.168.159.24', port=5000)
