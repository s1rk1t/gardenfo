#include <WiFi.h>
#include <ThingSpeak.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>

// Replace these with your network credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// ThingSpeak channel information
unsigned long channelID = YOUR_CHANNEL_ID; // Replace with your channel ID
const char* writeAPIKey = "YOUR_API_KEY";  // Replace with your Write API Key

// Pin assignments (update based on ESP32 capabilities)
const int moisturePin = 36;  // GPIO36 (ADC1_0) for Capacitive Soil Moisture Sensor
const int pHpin = 39;        // GPIO39 (ADC1_3) for Analog pH Sensor
const int ecPin = 34;        // GPIO34 (ADC1_6) for Analog EC Sensor
const int lightPin = 35;     // GPIO35 (ADC1_7) for LDR with a 10k resistor voltage divider
const int oneWireBus = 4;    // GPIO4 for DS18B20 temperature sensor
const int dhtPin = 5;        // GPIO5 for DHT22 humidity sensor

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
  Serial.begin(115200);
  
  // Initialize sensors
  tempSensor.begin();
  dht.begin();
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected!");

  ThingSpeak.begin(client);
}

void loop() {
  // Read sensor values
  int moistureValue = analogRead(moisturePin);  // Capacitive Soil Moisture Sensor
  int pHValue = analogRead(pHpin);             // Analog pH Sensor
  int ecValue = analogRead(ecPin);             // Analog EC Sensor
  float temperature = getTemperature();        // DS18B20 Temperature Sensor
  float humidityValue = dht.readHumidity();    // DHT22 Humidity Sensor
  int lightValue = analogRead(lightPin);       // LDR Photoresistor

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
