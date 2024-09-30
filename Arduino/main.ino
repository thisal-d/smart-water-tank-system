#include <WiFi.h> // Header for handle wifi
#include <HTTPClient.h> // Handle HTTP Requests
#include <ArduinoJson.h> // Convert String to Json object
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
long tankHeight = 314; // in cm
long tankFreeHeight = 10;
long usableTankHeight = tankHeight - tankFreeHeight;

double refillActivationPercentage = 0.5; // in %
double warningLevel1 = 0.8;
double warningLevel2 = 0.9;
double warningLevel3 = 1.0;
double waterLevelRate = 0.0;

// Capture status
long waterLevel = 0;
long waterMglValue = 0;
long distanceToWater = 0;
bool buzzerStatus =  false;
bool ledGreenStatus = false;
bool ledRedStatus = false;
bool waterRefilling = false;
String waterQuality = "";

// System Control variables
bool systemStatus = false; // default system status is on
bool pumpStatus = false;
bool pumpManualControlledStatus = false;

String pumpControlMode = "automatic";

// Wifi ssid and password
const char* ssid = "Redmi Note 11";
const char* password = "123456789";

// Server ip
const String serverUrl = "http://192.168.55.185:5000";


void setup(){
  // Initialize lcd display
  lcdDisplay.init();
  lcdDisplay.backlight();
  lcdDisplay.setCursor(0,0);
  lcdDisplay.print("SYSTEM : ");
  lcdDisplay.setCursor(0,1);
  lcdDisplay.print("INITIALIZING....");
  delay(1000);
  
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

  // Turn Off 
  turnOffDevice(ledRedPin, ledRedStatus);
  turnOffDevice(ledGreenPin, ledGreenStatus);
  turnOffDevice(warerPumpPin, pumpStatus);
  turnOffDevice(buzzerPin, buzzerStatus);

  // LCD :  wifi connecting
  lcdDisplay.clear();
  lcdDisplay.setCursor(0,0);
  lcdDisplay.print("WIFI : ");
  lcdDisplay.setCursor(0,1);
  lcdDisplay.print("CONNECTING....");
  // Connect to wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("WIFI : Connecting...");
  }
  // LCD : wifi connected
  lcdDisplay.clear();
  lcdDisplay.setCursor(0,0);
  lcdDisplay.print("WIFI : ");
  lcdDisplay.setCursor(0,1);
  lcdDisplay.print("CONNECTED....");
  delay(1000);
}


void displayStatus(){
  Serial.println("###############################################");
  Serial.println("System Status : " + String(systemStatus));
  Serial.println("Pump Status : " + String(pumpStatus));
  Serial.println("Pump Control Mode : " + String(pumpControlMode));
  Serial.println("Pump Manual Controlled Status : " + String(pumpManualControlledStatus));
  Serial.println("Buzzer Status : " + String(buzzerStatus));
  Serial.println("Led Green Status : " + String(ledGreenStatus));
  Serial.println("Led Red Status : " + String(ledRedStatus));
  Serial.println("Water Level : " + String(waterLevel));
  Serial.println("Water Mgl Value : " + String(waterMglValue));
  Serial.println("Distance To Water : " + String(distanceToWater));
  Serial.println("Water Refilling : " + String(waterRefilling));
  Serial.println("Water Filled Rate : " + String(waterLevelRate));
  Serial.println("Water Quality : " + waterQuality);
}

void loop(){
  displayStatus();
  // Every time check system status

  // Check  request is success or not
  int systemStatusTemp = getSystemStatus(serverUrl);
  if (systemStatusTemp != 2){ // Get status from server
      systemStatus = (systemStatusTemp == 1) ? true: false;
  }

  if (!systemStatus){
    turnOffDevice(ledRedPin, ledRedStatus);
    turnOffDevice(ledGreenPin, ledGreenStatus);
    turnOffDevice(buzzerPin, buzzerStatus);
    turnOffDevice(warerPumpPin, pumpStatus);
    turnOffDevice(ledRedPin, ledRedStatus);
    lcdDisplay.clear();
    lcdDisplay.setCursor(0, 0);
    lcdDisplay.print("SYSTEM STATUS:");
    lcdDisplay.setCursor(0, 1);
    lcdDisplay.print("SYSTEM OFF");
    delay(1000);
    return;
  }
  else{
    turnOnDevice(ledRedPin, ledRedStatus);
    delay(1000);
  }

  // Send data to server
  sendDeviceStatus(serverUrl, systemStatus, buzzerStatus, pumpStatus, ledGreenStatus, ledRedStatus, waterLevel, waterLevelRate, usableTankHeight, waterQuality);

  // If system in turn on
  turnOnDevice(ledRedPin, ledGreenStatus);

  waterMglValue = getWaterMglValue(tdsPin); // get the tds / mgl value of water
  waterQuality = getWaterQualityUsingMglValue(waterMglValue); // get the quality using tds / mgl value

  distanceToWater = getDistanceToWater(ultraSonicTrigPin, ultraSonicEchoPin); // get distance to the 
  waterLevel = tankHeight - distanceToWater;
  waterLevelRate = (double)waterLevel/usableTankHeight*100;


  // Display on LCD
  displayValuesOnLCD(lcdDisplay, waterLevelRate, waterLevel, waterQuality, waterMglValue, pumpStatus, pumpControlMode);

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
      waterRefilling = true;
      turnOnDevice(warerPumpPin, pumpStatus);
      turnOnDevice(ledGreenPin, ledGreenStatus);
    }
    else{
      waterRefilling = false;
      turnOffDevice(warerPumpPin, pumpStatus);
      turnOffDevice(ledGreenPin, ledGreenStatus);
    }
  }
  else{
    if (waterLevel >= usableTankHeight * warningLevel3){
      waterRefilling = false;
      turnOffDevice(warerPumpPin, pumpStatus);
      turnOffDevice(ledGreenPin, ledGreenStatus);
    }
    else if (waterLevel <= usableTankHeight * refillActivationPercentage || waterRefilling){
      waterRefilling = true;
      turnOnDevice(warerPumpPin, pumpStatus);
      turnOnDevice(ledGreenPin, ledGreenStatus);
    }
  }
  
  delay(1000);
}
