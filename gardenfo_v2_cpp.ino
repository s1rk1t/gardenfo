#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>

// Replace these with your network credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// ThingSpeak channel information
unsigned long channelID = YOUR_CHANNEL_ID; // Replace with your channel ID
const char* writeAPIKey = "YOUR_API_KEY";   // Replace with your Write API Key

// Pin assignments
int moisturePin = A0;    // Capacitive Soil Moisture Sensor
int pHpin = A1;          // Analog pH Sensor
int ecPin = A2;          // Analog EC Sensor
int lightPin = A3;       // LDR with a 10k resistor voltage divider
const int oneWireBus = 2; // DS18B20 temperature sensor pin
const int dhtPin = 3;     // DHT22 humidity sensor pin

// DS18B20 Temperature sensor setup
OneWire oneWire(oneWireBus);
DallasTemperature tempSensor(&oneWire);

// DHT22 setup
#define DHTTYPE DHT22
DHT dht(dhtPin, DHTTYPE);

// Threshold values for sensor readings
const int moistureThresholdLow = 300;  // Low moisture threshold (adjust based on calibration)
const int moistureThresholdHigh = 700; // High moisture threshold
const int pHThresholdLow = 5;          // Low pH threshold
const int pHThresholdHigh = 7;         // High pH threshold
const int ecThresholdLow = 200;        // Low EC threshold
const int ecThresholdHigh = 800;       // High EC threshold
const float tempThresholdLow = 18.0;   // Low temperature threshold (in °C)
const float tempThresholdHigh = 25.0;  // High temperature threshold
const int humidityThresholdLow = 40;   // Low humidity threshold (%)
const int humidityThresholdHigh = 60;  // High humidity threshold
const int lightThresholdLow = 300;     // Low light threshold
const int lightThresholdHigh = 800;    // High light threshold

WiFiClient client;

void setup() {
  Serial.begin(9600);
  
  // Initialize sensors
  tempSensor.begin();
  dht.begin();
  
  WiFi.begin(ssid, password);

  // Connect to Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  ThingSpeak.begin(client);
}

void loop() {
  // Read sensor values
  int moistureValue = analogRead(moisturePin);     // Capacitive Soil Moisture Sensor
  int pHValue = analogRead(pHpin);                 // Analog pH Sensor
  int ecValue = analogRead(ecPin);                 // Analog EC Sensor
  float temperature = getTemperature();            // DS18B20 Temperature Sensor
  float humidityValue = dht.readHumidity();        // DHT22 Humidity Sensor
  int lightValue = analogRead(lightPin);           // LDR Photoresistor

  // Print sensor values to Serial Monitor
  Serial.print("Moisture: "); Serial.print(moistureValue);
  Serial.print("\t pH: "); Serial.print(pHValue);
  Serial.print("\t EC: "); Serial.print(ecValue);
  Serial.print("\t Temperature: "); Serial.print(temperature);
  Serial.print("\t Humidity: "); Serial.print(humidityValue);
  Serial.print("\t Light: "); Serial.println(lightValue);

  // Check thresholds and take action
  checkThresholds(moistureValue, pHValue, ecValue, temperature, humidityValue, lightValue);

  // Send data to ThingSpeak
  ThingSpeak.setField(1, moistureValue);
  ThingSpeak.setField(2, pHValue);
  ThingSpeak.setField(3, ecValue);
  ThingSpeak.setField(4, temperature);
  ThingSpeak.setField(5, humidityValue);
  ThingSpeak.setField(6, lightValue);

  ThingSpeak.writeFields(channelID, writeAPIKey);
  delay(20000);  // Update every 20 seconds
}

// Function to read temperature from DS18B20
float getTemperature() {
  tempSensor.requestTemperatures();
  return tempSensor.getTempCByIndex(0); // Get temperature in Celsius from DS18B20
}

// Function to check sensor values against thresholds
void checkThresholds(int moisture, int pH, int ec, float temp, float humidity, int light) {
  // Check moisture
  if (moisture < moistureThresholdLow) {
    Serial.println("Warning: Moisture level is low! Consider watering.");
  } else if (moisture > moistureThresholdHigh) {
    Serial.println("Warning: Moisture level is high! Check drainage.");
  }

  // Check pH
  if (pH < pHThresholdLow) {
    Serial.println("Warning: pH level is low! Consider adding lime.");
  } else if (pH > pHThresholdHigh) {
    Serial.println("Warning: pH level is high! Consider adding sulfur.");
  }

  // Check EC
  if (ec < ecThresholdLow) {
    Serial.println("Warning: EC level is low! Consider adding nutrients.");
  } else if (ec > ecThresholdHigh) {
    Serial.println("Warning: EC level is high! Consider flushing with water.");
  }

  // Check temperature
  if (temp < tempThresholdLow) {
    Serial.println("Warning: Temperature is too low! Consider heating.");
  } else if (temp > tempThresholdHigh) {
    Serial.println("Warning: Temperature is too high! Consider cooling.");
  }

  // Check humidity
  if (humidity < humidityThresholdLow) {
    Serial.println("Warning: Humidity level is low! Consider misting.");
  } else if (humidity > humidityThresholdHigh) {
    Serial.println("Warning: Humidity level is high! Consider ventilation.");
  }

  // Check light
  if (light < lightThresholdLow) {
    Serial.println("Warning: Light level is low! Consider moving plants closer to light.");
  } else if (light > lightThresholdHigh) {
    Serial.println("Warning: Light level is high! Consider shading plants.");
  }
}

// Sensor Breakdown:
// Soil Moisture: Connected to analog pin A0 (Capacitive Soil Moisture Sensor).
// pH Sensor: Connected to analog pin A1 (Gravity pH Sensor).
// EC Sensor: Connected to analog pin A2 (Gravity EC Sensor).
// Temperature Sensor: DS18B20 connected via OneWire on pin 2.
// Humidity Sensor: DHT22 connected to pin 3 for both temperature and humidity, but we use the humidity value here.
// Light Sensor (LDR): Connected to analog pin A3 (Photoresistor with 10k resistor).