#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>
#include "Functions.ino"

// Pin Definitions
const int warerPumpPin = 4;
const int ledRedPin = 13;
const int buzzerPin = 14;
const int ledGreenPin = 25;
const int ultraSonicTrigPin = 26;
const int ultraSonicEchoPin = 27;
const int tdsPin = 35;


// LCD Dispaly 
// LCD I2C address and dimensions
#define LCD_ADDR 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2

// Initialzie the LCD display
LiquidCrystal_I2C lcdDisplay(LCD_ADDR, LCD_COLUMNS, LCD_ROWS);

// Water tank info
int tankHeight = 387; // in cm
int tankFreeHeight = 10;
int usableTankHeight = tankHeight - tankFreeHeight;

double refillActivationPercentage = 0.5; // in %
double warningLevel1 = 0.8;
double warningLevel2 = 0.9;
double warningLevel3 = 1.0;

// Capture status
int waterLevel = 0;
int waterMglValue = 0;
int distanceToWater = 0;
bool buzzerStatus =  false;
bool ledGreenStatus = false;
bool ledRedStatus = false;

// System Control variables
bool systemStatus = true; // default system status is on
bool pumpStatus = false;
bool pumpManualControlledStatus = false;

String pumpControlMode = "automatic";

// Wifi ssid and password
//const char* ssid = "SLT fibre";
// const char* password = "0912275170";
const char* ssid = "Redmi Note 11";
const char* password = "123456789" ;

// Server ip
const String serverUrl = "http://192.168.17.24:5000";

#define HIGH 0x0;
#define LOW 0x1;

void setup(){
  // Initialize output pins
  pinMode(warerPumpPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
  pinMode(ledRedPin, OUTPUT);
  pinMode(ultraSonicTrigPin, OUTPUT);

  // Initialize input pins
  pinMode(ultraSonicEchoPin, INPUT);
  pinMode(tdsPin, INPUT);

  // Initialize serial monitor
  Serial.begin(115200);

  // Initialize lcd display
  lcdDisplay.begin(LCD_COLUMNS, LCD_ROWS);
  lcdDisplay.backlight();
  lcdDisplay.setCursor(0,0);

  // Turn Off 
  turnOffDevice(ledRedPin, ledRedStatus);
  turnOffDevice(ledGreenPin, ledGreenStatus);
  turnOffDevice(warerPumpPin, pumpStatus);
  turnOffDevice(buzzerPin, buzzerStatus);

  // Connect to wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("WIFI : Connecting...");
  }
  Serial.println("WIFI : Connected");
}


void displayStatus(){
  Serial.println("SystemStatus : " + String(systemStatus));
  Serial.println("-----------------------------------------------");
  Serial.println("pumpStatus : " + String(pumpStatus));
  Serial.println("pumpControlMode : " + String(pumpControlMode));
  Serial.println("pumpManualControlledStatus : " + String(pumpManualControlledStatus));
  Serial.println("-----------------------------------------------");
  Serial.println("buzzerStatus : " + String(buzzerStatus));
  Serial.println("ledGreenStatus : " + String(ledGreenStatus));
  Serial.println("ledRedStatus : " + String(ledRedStatus));
  Serial.println("-----------------------------------------------");
  Serial.println("waterLevel : " + String(waterLevel));
  Serial.println("waterMglValue : " + String(waterMglValue));
  Serial.println("distanceToWater : " + String(distanceToWater));
  Serial.println("###############################################");
}

void loop(){
  displayStatus();
  // Every time check system status

  // Check  request is success or not
  int systemStatusTemp = getSystemStatus(serverUrl);
  if (systemStatusTemp != 2){ // Get status from server
      systemStatus = (systemStatusTemp == 1) ? true: false;
  }
  // Send data to server
  sendDeviceStatus(serverUrl, systemStatus, buzzerStatus, pumpStatus, ledGreenStatus, ledRedStatus, waterLevel, waterMglValue);
  
  if (!systemStatus){
    turnOffDevice(ledRedPin, ledRedStatus);
    turnOffDevice(ledGreenPin, ledGreenStatus);
    turnOffDevice(buzzerPin, buzzerStatus);
    turnOffDevice(warerPumpPin, pumpStatus);
    turnOffDevice(ledRedPin, ledRedStatus);
    delay(1000);
    return;
  }
  else{
    turnOnDevice(ledRedPin, ledRedStatus);
  }

  // If system in turn on
  turnOnDevice(ledRedPin, ledGreenStatus);

  waterMglValue = getWaterMglValue(tdsPin);
  distanceToWater = getDistanceToWater(ultraSonicTrigPin, ultraSonicEchoPin);
  waterLevel = tankHeight - distanceToWater;

  // Check  request is success or not
  String pumpControlModeTemp = getPumpControlMethod(serverUrl);
  if (pumpControlModeTemp != "unknown"){ // Get status from server
    pumpControlMode = pumpControlModeTemp;
  }
  
  if (pumpControlMode == "manual"){
    int pumpManualControlledStatusTemp = getPumpManualControlledStatus(serverUrl);
    if (pumpManualControlledStatusTemp != 2){
      pumpManualControlledStatus = (pumpManualControlledStatusTemp == 1) ? true: false;
    }
  }
  
  if ((waterLevel >= usableTankHeight * warningLevel1) && pumpStatus){
    buzzerStatus = true;
    sendBuzzerStatus(serverUrl, buzzerStatus);
    turnOnBuzzer(waterLevel, usableTankHeight, buzzerPin, warningLevel1, warningLevel2, warningLevel3);
  }
  else{
    buzzerStatus = false;
  }

  if (pumpControlMode=="manual"){
    if (pumpManualControlledStatus){
      turnOnDevice(warerPumpPin, pumpStatus);
      turnOnDevice(ledGreenPin, ledGreenStatus);
    }
    else{
      turnOffDevice(warerPumpPin, pumpStatus);
      turnOffDevice(ledGreenPin, ledGreenStatus);
    }
  }
  else{
    if (waterLevel >= usableTankHeight * warningLevel3){
      turnOffDevice(warerPumpPin, pumpStatus);
      turnOffDevice(ledGreenPin, ledGreenStatus);
    }
    else if (waterLevel <= usableTankHeight * refillActivationPercentage){
      turnOnDevice(warerPumpPin, pumpStatus);
      turnOnDevice(ledGreenPin, ledGreenStatus);
    }
  }
  
  delay(1000);
}
