const SERVER_URL = 'http://192.168.250.24:5000';
const SYSTEM_CONTROL_BUTTON = document.getElementById("system-control-button")


async function fetchStatus() {
    try {
        const response = await fetch(`${SERVER_URL}/status`);
        if (!response.ok) {
            throw new Error('Network response was not ok');
        }
        const data = await response.json();
        document.getElementById('systemStatus').textContent = data.system_status;
        document.getElementById('buzzerStatus').textContent = data.buzzer_status;
        document.getElementById('pumpStatus').textContent = data.pump_status;
        document.getElementById('ledStatus').textContent = data.led_status;
        document.getElementById('distanceToWater').textContent = data.distance_to_water;
        document.getElementById('waterPhValue').textContent = data.water_ph_value;
        console.log("Data:", data);
    } catch (error) {
        console.error('Error fetching status:', error);
    }
}

async function toggleSystem() {
    try {
        const response = await fetch(`${SERVER_URL}/toggle`, { method: 'POST' });
    } catch (error) {
        console.error('Error toggling system:', error);
    }
}


SYSTEM_CONTROL_BUTTON.addEventListener("click", toggleSystem)

setInterval(fetchStatus, 1000);

fetchStatus();