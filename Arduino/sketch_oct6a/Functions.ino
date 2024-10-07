#ifndef Functions
#define Functions  

///////////////////////////////////////////////////////////////////////////////////////////////
// Get distance to water using ultra sonic sensor
int getDistanceToWater(int ultraSonicTrigPin, int ultraSonicEchoPin) {
  // Send a pulse to trigger
  digitalWrite(ultraSonicTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(ultraSonicTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultraSonicTrigPin, LOW);

  // Measure the duration of the pulse
  int duration = pulseIn(ultraSonicEchoPin, HIGH);

  // Convert duration to distance in cm
  int distance = (duration / 2) / 29.1;

  return distance;
};

///////////////////////////////////////////////////////////////////////////////////////////////
// Get water MGL
int getWaterMglValue(int tdsPin){
    int electricalConductivity = analogRead(tdsPin);
    return electricalConductivity;
};

//////////////////////////////////////////////////////////////////////////////////////////////
// Get water quality status
String getWaterQualityUsingMglValue(int waterMglValue){
  String waterQuality = "";
  if (waterMglValue <= 300) waterQuality = "good";
  else if (waterMglValue <= 600) waterQuality = "poor";
  else waterQuality = "critical";
  return waterQuality;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Display LCD
void displayValuesOnLCD(LiquidCrystal_I2C lcdDisplay, double waterLevelRate, int waterLevel, String waterQuality, int waterMglValue, bool pumpStatus, String pumpControlMode){
  int changeDelay = 1000;
  lcdDisplay.clear();

  // Display water level
  // Firs Row
  lcdDisplay.setCursor(0, 0);
  lcdDisplay.print("WATER LEVEL :");
  // Second Row
  lcdDisplay.setCursor(0, 1);
  lcdDisplay.print((int)waterLevelRate);
  lcdDisplay.print("%");
  lcdDisplay.print(" ");
  lcdDisplay.print(waterLevel);
  lcdDisplay.print("CM");
  delay(changeDelay);

  // Display water quality
  lcdDisplay.clear();
  lcdDisplay.setCursor(0, 0);
  lcdDisplay.print("WATER QUALITY :");
  lcdDisplay.setCursor(0, 1);
  lcdDisplay.print(waterQuality == "low" ? "LOW" : waterQuality == "critical" ?  "CRITICAL" : "GOOD");
  lcdDisplay.print(" ");
  lcdDisplay.print(waterMglValue);
  lcdDisplay.print("MGL");
  delay(changeDelay);

  // Display pump status
  lcdDisplay.clear();
  lcdDisplay.setCursor(0, 0);
  lcdDisplay.print("PUMP STATUS :");
  lcdDisplay.setCursor(0, 1);
  lcdDisplay.print(pumpControlMode == "manual" ? "MANUAL" : "AUTOMATIC");
  lcdDisplay.print(" ");
  lcdDisplay.print(pumpStatus? "ON" : "OFF");
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Send device status
void sendDeviceStatus(
    String serverUrl, bool system_status, bool buzzer_status, bool pump_status, 
    bool led_green_status, bool led_red_status,int water_level, double water_level_rate, 
    int tankHeight, String waterQuality
  ){

  HTTPClient http;
  http.begin(serverUrl+"/send-status");
  http.addHeader("Content-Type", "application/json");

  String payLoad = "{\"system_status\":\"" + String(system_status) + "\","
              + "\"buzzer_status\":\"" + String(buzzer_status) + "\","
              + "\"pump_status\":\"" + String(pump_status) + "\","
              + "\"led_green_status\":\"" + String(led_green_status) + "\","
              + "\"led_red_status\":\"" + String(led_red_status) + "\","
              + "\"water_level\":\"" + String(water_level) + "\","
              + "\"water_level_rate\":\"" + String(water_level_rate) + "\","
              + "\"water_quality\":\"" + String(waterQuality) + "\","
              + "\"tank_height\":\"" + String(tankHeight) + "\"}";

  int httpResponseCode = http.POST(payLoad);

  if (httpResponseCode > 0) {
    String response = http.getString();
    // Serial.println("Status Update Response Code: " + String(httpResponseCode));
    // Serial.println(response);
  } else {
    // Serial.print("Error on sending POST: ");
    // Serial.println(httpResponseCode);
  }
  http.end();
};

///////////////////////////////////////////////////////////////////////////////////////////////
// Send buzzer status
void sendBuzzerStatus(String serverUrl, bool buzzer_status){   
  HTTPClient http;
  http.begin(serverUrl+"/send-buzzer-status");
  http.addHeader("Content-Type", "application/json");

  String payLoad = "{\"buzzer_status\":\"" + String(buzzer_status) +"\"}";

  int httpResponseCode = http.POST(payLoad);

  if (httpResponseCode > 0) {
    String response = http.getString();
    // Serial.println("Status Update Response Code: " + String(httpResponseCode));
    // Serial.println(response);
  } else {
    // Serial.print("Error on sending POST: ");
    // Serial.println(httpResponseCode);
  }
  http.end();
};

///////////////////////////////////////////////////////////////////////////////////////////////
// Get system State/ Pump control method/ manual controlled pump status
void checkSystemControlInfo(String serverUrl,bool& systemStatus, String& pumpControlMode, bool& pumpManualControlledStatus){
  HTTPClient http;
  http.begin(serverUrl + "/get-system-control-status");
  int httpResponseCode = http.GET();
  if  (httpResponseCode>0){
    String payload = http.getString();
    // Serial.println(payload);
    DynamicJsonDocument jsonObj(1024); // Create json obj
    deserializeJson(jsonObj, payload);
    systemStatus = jsonObj["system_status"].as<bool>();
    pumpControlMode = jsonObj["pump_control_method"].as<String>();
    pumpManualControlledStatus = jsonObj["pump_manual_controlled_status"].as<bool>();
    http.end();
  }
  http.end();
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Power on device and change the status of device to True
void turnOnDevice(int devicePin, bool &deviceStatus){
  digitalWrite(devicePin, HIGH);
  deviceStatus = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Power off device and change the status of device to False
void turnOffDevice(int devicePin, bool &deviceStatus){
  digitalWrite(devicePin, LOW);
  deviceStatus = false;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Handle the buzzer sound effects
void turnOnBuzzer(int waterLevel, int usableTankHeight, int buzzerPin, double warningLevel1, double warningLevel2, double warningLevel3){
  int delayTime = 0;
  int delayTime2 = 0;
  int repeats = 0;
  if (waterLevel >= warningLevel3 * usableTankHeight) 
    {delayTime = 100; 
    repeats = 5;
  }
  else if (waterLevel >= warningLevel2 * usableTankHeight) {
    delayTime = 400;
    repeats = 3;
  }
  else if (waterLevel >= warningLevel1 * usableTankHeight) {
    delayTime = 2000; 
    repeats = 1;
  } 
  for (int i=1; i<=repeats; i++){
    digitalWrite(buzzerPin, HIGH);
    delay(delayTime);
    digitalWrite(buzzerPin, LOW);
    if (i!=repeats) delay(delayTime2); // Only delay if loops working, no waiting time if last loop time
  }
}



#endif 
