#ifndef Functions
#define Functions  

///////////////////////////////////////////////////////////////////////////////////////////////
// Get distance to water using ultra sonic sensor
long getDistanceToWater(int ultraSonicTrigPin, int ultraSonicEchoPin) {
    digitalWrite(ultraSonicTrigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(ultraSonicTrigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(ultraSonicTrigPin, LOW);
    return (pulseIn(ultraSonicEchoPin, HIGH) / 2) / 29.1;
  };

///////////////////////////////////////////////////////////////////////////////////////////////
// Get water MGL
long getWaterMglValue(int tdsPin){
    long electricalConductivity = analogRead(tdsPin);
    return electricalConductivity;
};

///////////////////////////////////////////////////////////////////////////////////////////////
// Send device status
void sendDeviceStatus(String serverUrl, bool system_status, bool buzzer_status, bool pump_status, bool led_green_status, bool led_red_status,int water_level, int water_mgl_value){   
  HTTPClient http;
  http.begin(serverUrl+"/send-status");
  http.addHeader("Content-Type", "application/json");

  String payLoad = "{\"system_status\":\"" + String(system_status) + "\","
              + "\"buzzer_status\":\"" + String(buzzer_status) + "\","
              + "\"pump_status\":\"" + String(pump_status) + "\","
              + "\"led_green_status\":\"" + String(led_green_status) + "\","
              + "\"led_red_status\":\"" + String(led_red_status) + "\","
              + "\"water_level\":\"" + String(water_level) + "\","
              + "\"water_mgl_value\":\"" + String(water_mgl_value) + "\"}";

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

  String payLoad = "{\"buzzer_status\":\"" + String(buzzer_status) + "}";

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
// Retrieve system status
int getSystemStatus(String serverUrl){
    HTTPClient http;
    http.begin(serverUrl + "/get-system-status");
    int httpResponseCode = http.GET();
    int system_status;
    // httpResponseCode is the HTTP response code from the server. if it more than 0 then it was successful
    if  (httpResponseCode>0){
        //Serial.println("Request Success : " + String(httpResponseCode));
        String payload = http.getString();
        //Serial.println(payload);
        DynamicJsonDocument jsonObj(1024);
        deserializeJson(jsonObj, payload);
        system_status = jsonObj["system_status"].as<int>();
        //Serial.println(system_status);
    }
    else{
        //Serial.println("Request Failed : " + String(httpResponseCode));
        system_status = 2;
    }
    http.end();
    return system_status;
};

///////////////////////////////////////////////////////////////////////////////////////////////
// Retrieve pump control method
String getPumpControlMethod(String serverUrl){
  HTTPClient http;
  http.begin(serverUrl + "/get-pump_control_method");
  int httpResponseCode = http.GET();
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
int getPumpManualControlledStatus(String serverUrl){
  HTTPClient http;
  http.begin(serverUrl + "/get-pump-manual-controlled-status");
  int httpResponseCode = http.GET();
  int pumpStatus;
  // httpResponseCode is the HTTP response code from the server. if it more than 0 then it was successful
  if  (httpResponseCode>0){
      String payload = http.getString();
      //Serial.println(payload);
      DynamicJsonDocument jsonObj(1024);
      deserializeJson(jsonObj, payload);
      pumpStatus = jsonObj["pump_manual_controlled_status"].as<int>();
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
void turnOnDevice(int devicePin, bool &deviceStatus){
  digitalWrite(devicePin, LOW);
  deviceStatus = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Power off device and change the status of device to False
void turnOffDevice(int devicePin, bool &deviceStatus){
  digitalWrite(devicePin, HIGH);
  deviceStatus = false;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Handle the buzzer sound effects
void turnOnBuzzer(int waterLevel, int usableTankHeight, int buzzerPin, double warningLevel1, double warningLevel2, double warningLevel3){
  int delayTime = 0;
  if (waterLevel >= warningLevel3 * usableTankHeight)  delayTime = 100;
  else if (waterLevel >= warningLevel2 * usableTankHeight)  delayTime = 400;
  else if (waterLevel >= warningLevel1 * usableTankHeight)  delayTime = 800;
  for (int i=0; i<5; i++){
    digitalWrite(buzzerPin, LOW);
    delay(delayTime);
    digitalWrite(buzzerPin, HIGH);
    delay(delayTime);
  }
}
#endif 