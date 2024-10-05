#ifndef Functions
#define Functions  

///////////////////////////////////////////////////////////////////////////////////////////////
// Get distance to water using ultra sonic sensor
long getDistanceToWater(long ultraSonicTrigPin, long ultraSonicEchoPin) {
  // Send a pulse to trigger
  digitalWrite(ultraSonicTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(ultraSonicTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultraSonicTrigPin, LOW);

  // Measure the duration of the pulse
  long duration = pulseIn(ultraSonicEchoPin, HIGH);

  // Convert duration to distance in cm
  long distance = (duration / 2) / 29.1;

  return distance;
};

///////////////////////////////////////////////////////////////////////////////////////////////
// Get water MGL
long getWaterMglValue(long tdsPin){
    long electricalConductivity = analogRead(tdsPin);
    return electricalConductivity;
};

//////////////////////////////////////////////////////////////////////////////////////////////
// Get water quality status
String getWaterQualityUsingMglValue(long waterMglValue){
  String waterQuality = "";
  if (waterMglValue <= 300) waterQuality = "good";
  else if (waterMglValue <= 600) waterQuality = "poor";
  else waterQuality = "critical";
  return waterQuality;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Display LCD
void displayValuesOnLCD(LiquidCrystal_I2C lcdDisplay, double waterLevelRate, long waterLevel, String waterQuality, long waterMglValue, bool pumpStatus, String pumpControlMode){
  long changeDelay = 1000;
  lcdDisplay.clear();

  // Display water level
  // Firs Row
  lcdDisplay.setCursor(0, 0);
  lcdDisplay.print("WATER LEVEL :");
  // Second Row
  lcdDisplay.setCursor(0, 1);
  lcdDisplay.print((long)waterLevelRate);
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
void sendDeviceStatus(String serverUrl, bool system_status, bool buzzer_status, bool pump_status, bool led_green_status, bool led_red_status,long water_level, double water_level_rate, long tankHeight, String waterQuality){   
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

  long httpResponseCode = http.POST(payLoad);

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

  long httpResponseCode = http.POST(payLoad);

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
// Retrieve system status
long getSystemStatus(String serverUrl){
    HTTPClient http;
    http.begin(serverUrl + "/get-system-status");
    long httpResponseCode = http.GET();
    long system_status;
    // httpResponseCode is the HTTP response code from the server. if it more than 0 then it was successful
    if  (httpResponseCode>0){
        //Serial.println("Request Success : " + String(httpResponseCode));
        String payload = http.getString();
        //Serial.println(payload);
        DynamicJsonDocument jsonObj(1024);
        deserializeJson(jsonObj, payload);
        system_status = jsonObj["system_status"].as<long>();
        //Serial.println(system_status);
    }
    else{
        Serial.println("Request Failed : " + String(httpResponseCode));
        system_status = 2;
    }
    http.end();
    return system_status;
};

///////////////////////////////////////////////////////////////////////////////////////////////
// Retrieve pump control method
String getPumpControlMethod(String serverUrl){
  HTTPClient http;
  http.begin(serverUrl + "/get-pump-control-method");
  long httpResponseCode = http.GET();
  String pumpControlMethod;
  // httpResponseCode is the HTTP response code from the server. if it more than 0 then it was successful
  if  (httpResponseCode>0){
      String payload = http.getString();
      //Serial.println(payload);
      DynamicJsonDocument jsonObj(1024);
      deserializeJson(jsonObj, payload);
      pumpControlMethod = jsonObj["pump_control_method"].as<String>();
      //Serial.println(pumpControlMethod);
      http.end();
  }
  else{
    pumpControlMethod = "unknown";
  }
  http.end();
  return pumpControlMethod;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Retrieve pump manual controlled status
long getPumpManualControlledStatus(String serverUrl){
  HTTPClient http;
  http.begin(serverUrl + "/get-pump-manual-controlled-status");
  long httpResponseCode = http.GET();
  long pumpStatus;
  // httpResponseCode is the HTTP response code from the server. if it more than 0 then it was successful
  if  (httpResponseCode>0){
      String payload = http.getString();
      //Serial.println(payload);
      DynamicJsonDocument jsonObj(1024);
      deserializeJson(jsonObj, payload);
      pumpStatus = jsonObj["pump_manual_controlled_status"].as<long>();
      //Serial.println(pumpStatus);
      http.end();
  }
  else{
    pumpStatus = 2;
  }
  http.end();
  //Serial.println("Failed");
  return pumpStatus;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Power on device and change the status of device to True
void turnOnDevice(long devicePin, bool &deviceStatus){
  digitalWrite(devicePin, HIGH);
  deviceStatus = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Power off device and change the status of device to False
void turnOffDevice(long devicePin, bool &deviceStatus){
  digitalWrite(devicePin, LOW);
  deviceStatus = false;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Handle the buzzer sound effects
void turnOnBuzzer(long waterLevel, long usableTankHeight, long buzzerPin, double warningLevel1, double warningLevel2, double warningLevel3){
  long delayTime = 0;
  long repeats = 0;
  if (waterLevel >= warningLevel3 * usableTankHeight) 
    {delayTime = 100; 
    repeats = 5;
  }
  else if (waterLevel >= warningLevel2 * usableTankHeight) {
    delayTime = 400;
    repeats = 3;
  }
  else if (waterLevel >= warningLevel1 * usableTankHeight) {
    delayTime = 800; 
    repeats = 1;
  } 
  for (long i=0; i<repeats; i++){
    digitalWrite(buzzerPin, HIGH);
    delay(delayTime);
    digitalWrite(buzzerPin, LOW);
    delay(delayTime);
  }
}
#endif 
