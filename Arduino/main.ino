#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define BLYNK_TEMPLATE_ID "TMPL6wJElr1n4"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "2POexugO2YdI1_IPFMtxgqGUnnGqHYC7"
#include <BlynkSimpleEsp32.h>

char auth[] = "2POexugO2YdI1_IPFMtxgqGUnnGqHYC7";
const char* ssid = "Redmi Note 11";
const char* password = "123456789";
const char* serverName = "http://192.168.159.24:5000/update"; // Replace with your Flask server IP
const char* toggleURL = "http://192.168.159.24:5000/get_system"; // Endpoint to toggle system state

const int buzzerPin = 25;
const int pumpPin = 4;
const int ledPin = 14;
const int tdsPin = 34;
const int trigPin = 26;
const int echoPin = 27;

String ledDisplayStatus = "OFF";
String buzzerDisplayStatus = "OFF";
String pumpDisplayStatus = "OFF";
String systemDisplayStatus = "OFF";
String distanceToWaterDisplay = "0 cm";
String waterPhValueDisplay = "0 pH";

#define VIRTUAL_WATER_LEVEL_PIN V0
#define VIRTUAL_PUMP_CONTROL_PIN V1
#define VIRTUAL_TDS_PIN V3

bool systemState = true;

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(tdsPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(115200);

  digitalWrite(buzzerPin, LOW);
  digitalWrite(ledPin, LOW);

  Blynk.begin(auth, ssid, password);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void displayStatus() {
  Serial.println("System Status : " + systemDisplayStatus);
  Serial.println("Buzzer Status : " + buzzerDisplayStatus);
  Serial.println("Pump Status : " + pumpDisplayStatus);
  Serial.println("LED Status : " + ledDisplayStatus);
  Serial.println("Distance To Water : " + distanceToWaterDisplay);
  Serial.println("Ph Value : " + waterPhValueDisplay);
}

float readTdsSensorPhValue() {
  const double vRef = 3.3;
  const double neutralVoltage = 1.65;
  const double acidVoltage = 2.5;
  const double baseVoltage = 0.8;
  int phValueRaw = analogRead(tdsPin);
  double voltage = phValueRaw * (vRef / 4095.0);
  double slope = (10.0 - 4.0) / (baseVoltage - acidVoltage);
  double intercept = 7.0 - slope * neutralVoltage;
  return slope * voltage + intercept;
}

long readUltrasonicDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return (pulseIn(echoPin, HIGH) / 2) / 29.1;
}

void sendStatusUpdate() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    float waterPhLevel = readTdsSensorPhValue();
    long distanceToWater = readUltrasonicDistance();

    String jsonPayload = "{\"system_status\":\"" + systemDisplayStatus + "\","
                    + "\"buzzer_status\":\"" + buzzerDisplayStatus + "\","
                    + "\"pump_status\":\"" + pumpDisplayStatus + "\","
                    + "\"led_status\":\"" + ledDisplayStatus + "\","
                    + "\"distance_to_water\":\"" + distanceToWaterDisplay + "\","
                    + "\"water_ph_value\":\"" + waterPhValueDisplay + "\"}";
    int httpResponseCode = http.POST(jsonPayload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Status Update Response Code: " + String(httpResponseCode));
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  }
}

void toggleSystem() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    char * s = "http://192.168.159.24:5000/get_system";
    http.begin(s);

    int httpCode = http.GET(); // Make the GET request

    if (httpCode > 0) {
      String payload = http.getString(); // Get the response payload
      Serial.println("Response Code: " + String(httpCode));
      Serial.println("Response Payload: " + payload);
       // Create a DynamicJsonDocument to parse the JSON response
      DynamicJsonDocument doc(1024); // Adjust the size as needed

      DeserializationError error = deserializeJson(doc, payload);

      if (error) {
        Serial.println("Failed to parse JSON");
        return;
      }


      // Extract the values from the JSON document
      String systemStateString = doc["system_status"].as<String>();
      Serial.println("Response systemStateString: " + systemStateString);

      if (systemStateString == "true") {
        systemState = true;
        systemDisplayStatus = "ON";
      } else {
        systemState = false;
        systemDisplayStatus = "OFF";
      }
      // Optionally, parse the JSON response if needed
      // parseJSON(payload);
    } else {
      Serial.println("Error on HTTP request");
    }

    http.end();
  } else {
    Serial.println("Not connected to WiFi");
  }
}

void loop() {
  Blynk.run();

  // Check if a command is available via Serial Monitor
  
  toggleSystem();  // Toggle system state via web request

  sendStatusUpdate();
  displayStatus();

  long distanceToWater = readUltrasonicDistance(); // Distance in cm
  distanceToWaterDisplay = String(distanceToWater) + " cm";

  float waterPhLevel = readTdsSensorPhValue();
  waterPhValueDisplay = String(waterPhLevel) + " pH";

  Blynk.virtualWrite(VIRTUAL_TDS_PIN, waterPhLevel);
  Blynk.virtualWrite(VIRTUAL_WATER_LEVEL_PIN, distanceToWater);

  if (systemState) {
    digitalWrite(pumpPin, HIGH);
    if (distanceToWater <= 5) {
      digitalWrite(buzzerPin, LOW);
      digitalWrite(ledPin, HIGH);
      ledDisplayStatus = "OFF";
      buzzerDisplayStatus = "ON";
      pumpDisplayStatus = "OFF";
    } else {
      digitalWrite(buzzerPin, HIGH);
      digitalWrite(ledPin, LOW);
      ledDisplayStatus = "ON";
      buzzerDisplayStatus = "OFF";
      pumpDisplayStatus = "ON";
    }
  } else {
    digitalWrite(pumpPin, LOW);
    digitalWrite(ledPin, HIGH);
    ledDisplayStatus = "OFF";
    buzzerDisplayStatus = "OFF";
    pumpDisplayStatus = "OFF";
  }

  delay(1000); // Wait 1 second before next read
}

BLYNK_WRITE(VIRTUAL_PUMP_CONTROL_PIN) {
  systemState = param.asInt(); // Get the value of the virtual pin
  systemDisplayStatus = systemState ? "ON" : "OFF";
  if (!systemState) {
    ledDisplayStatus = "OFF";
    buzzerDisplayStatus = "OFF";
    pumpDisplayStatus = "OFF";
  }
}
