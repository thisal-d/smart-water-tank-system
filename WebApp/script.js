const SERVER = "http://192.168.133.185";
const PORT_ADDRESS = ':5000';
const SERVER_URL = SERVER + PORT_ADDRESS;

const waterLevelRateEl = document.getElementById("water-level-rate-value");
const waterQualityEL = document.getElementById("water-quality-value");
const systemStatusEl = document.getElementById("system-status-value");
const pumpStatusEl = document.getElementById("pump-status-value");
const buzzerStatusEl = document.getElementById("buzzer-status-value");
const serverConnectionStatusEl = document.getElementById("server-connection-status");
const deviceStatusEl = document.getElementById("device-status");
const serverConnectionStatusCircleEl = document.getElementById("server-connection-status-circle");
const deviceStatusCircleEl = document.getElementById("device-status-circle");
const systemControlButton = document.getElementById("system-control-button");
const pumpControlButton = document.getElementById("pump-control-button");
const statusContainerEl = document.getElementById("status-container");

const waterLevelValueEl = document.getElementById("water-level-value-cm");
const tankHeightValueEl = document.getElementById("tank-height-value-cm");     

let systemStates = false;
let waterLevelRate = 0.0;
let waterQuality = "critical";
let deviceStatus = null;
let serverConnectionStatus = false;
let pumpControlMode = 'automatic';

const RED = "#FF0000"; 
const GREEN = "#00FF00";
const DARK_GREEN = "#23e027"; 
const DARK_YELLOW = "#fcd705"; 
const GREY = "#404040";
const OPACITY_INACTIVE = 0.5;
const OPACITY_ACTIVE = 1;

// Handle the pump turn off on and automatic
const changePumpControlMode = async () => {
    try{
        // Get Current Pump Control Mode
        const response = await fetch(`${SERVER_URL}/get-system-control-status`);
        const data = await response.json()
        let currentPumpControlMode = data["pump_control_method"];
        let currentPumpManualControlledStatus = data["pump_manual_controlled_status"];

        let requestUlr = '';
        if (currentPumpControlMode=='automatic') requestUlr = 'turn-on-pump-manually';
        else if (currentPumpControlMode=='manual' && currentPumpManualControlledStatus) requestUlr = 'turn-off-pump-manually';
        else requestUlr = 'turn-on-automatic-pump-control-mode';
        
        // Control the pump
        try {
            await fetch(`${SERVER_URL}/${requestUlr}`, { method: 'POST' });
        } catch (error) {
            console.log('Pump Control Error (Send Data):', error);
        }

    } catch (error) {
        console.log('Pump Control Error :', error);
    }
}

// Handle the system turn on and off
const systemTurnOnOff = async () => {
    try {
        const response = await fetch(`${SERVER_URL}/get-system-control-status`);
        const data = await response.json();
        let currentSystemStatus = data["system_status"];
        let requestUrl = currentSystemStatus? "turn-off-system" : "turn-on-system";

        try{
            await fetch(`${SERVER_URL}/${requestUrl}`, { method: 'POST' });
        } catch (error){
            console.log("System Off/On Error :", error);
        }
    } catch (error) {
        console.log('System Off/On Error (Request Data):', error);
    }
};

const updateUi = () => {
    // output container opacity
    if (systemStates)statusContainerEl.style.opacity = OPACITY_ACTIVE;
    else statusContainerEl.style.opacity = OPACITY_INACTIVE;

    // water level rate color
    if (waterLevelRate > 100) waterLevelRateEl.style.color = RED;
    else if (waterLevelRate > 50) waterLevelRateEl.style.color = DARK_GREEN;
    else if (waterLevelRate > 25) waterLevelRateEl.style.color = DARK_YELLOW;
    else waterLevelRateEl.style.color = RED;

    // water quality color
    if (waterQuality=="good") waterQualityEL.style.color = GREEN;
    else if (waterQuality=="poor") waterQualityEL.style.color = DARK_YELLOW;
    else waterQualityEL.style.color = RED;
    
    // device status color
    if (deviceStatus) deviceStatusCircleEl.style.backgroundColor = GREEN;
    else if (deviceStatus==false) deviceStatusCircleEl.style.backgroundColor = RED;
    else deviceStatusCircleEl.style.backgroundColor = GREY;
    

    if (serverConnectionStatus==true) serverConnectionStatusCircleEl.style.backgroundColor = GREEN;
    else if (serverConnectionStatus==false) serverConnectionStatusCircleEl.style.backgroundColor = RED;
    else serverConnectionStatusCircleEl.style.backgroundColor = GREY;

    if (pumpControlMode=='automatic') pumpControlButton.textContent = "Manual/On";
    else if (pumpControlMode=='manual' && pumpManualCrontrolledStatus==true) pumpControlButton.textContent = "Manual/Off";
    else pumpControlButton.textContent = "Automatic";

    if (systemStates) systemControlButton.textContent = "OFF";
    else systemControlButton.textContent = "ON";
}

// Fetch data from server
async function fetchStatus() {
    try {
        const response = await fetch(`${SERVER_URL}/get-status`);
        if (!response.ok) {
            throw new Error('Network response was not ok');
        }
        const data = await response.json();
        

        waterLevelRateEl.textContent = data["water_level_rate"] + " %";
        systemStatusEl.textContent = data["system_status"]? "ON" : "OFF";
        pumpStatusEl.textContent = data["pump_status"]? "ON" : "OFF";
        buzzerStatusEl.textContent = data["buzzer_status"]? "ON" : "OFF";
        deviceStatusEl.textContent = data["device_status"]? "Online" : "Offline";
        waterQualityEL.textContent = data["water_quality"] == "good" ? "Good 🙂" : data["water_quality"] == "poor"? "Poor 😐" : "Critical ☹️";        
        waterLevelValueEl.textContent = data["water_level"] + " cm";
        tankHeightValueEl.textContent = data["tank_height"] + " cm";
        serverConnectionStatusEl.textContent = "Online";

        pumpControlMode = data["pump_control_method"];
        pumpManualCrontrolledStatus = data["pump_manual_controlled_status"];
        waterLevelRate = data["water_level_rate"];
        deviceStatus = data["device_status"];
        systemStates = data["system_status"];
        waterQuality = data["water_quality"];


        serverConnectionStatus = true;
        updateUi();
        
        // console.log(data);
    } catch (error) {
        deviceStatus = null;
        serverConnectionStatus = false;
        serverConnectionStatusEl.textContent = "Offline";
        deviceStatusEl.textContent = "Unknown";
        updateUi();
        console.error('Error fetching status:', error);
    }
}


systemControlButton.addEventListener('click', systemTurnOnOff);
pumpControlButton.addEventListener('click', changePumpControlMode);

// Fetch status every 1 second
setInterval(fetchStatus, 1000);

fetchStatus();
