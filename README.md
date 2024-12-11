# Gardinfo Project

Gardinfo is an Arduino-based project designed to monitor key environmental factors in a garden. It collects data such as soil moisture, pH, electrical conductivity (EC), temperature, humidity, and light intensity, and uploads the data to ThingSpeak for real-time monitoring and analysis. The project is designed to help gardeners optimize plant health and improve their gardening strategies.

---

## Features

1. **Environmental Monitoring**:
   - **Soil Moisture:** Measures the water content of the soil.
   - **pH Levels:** Monitors soil acidity or alkalinity.
   - **Electrical Conductivity (EC):** Tracks nutrient levels in the soil.
   - **Temperature:** Reads the ambient temperature.
   - **Humidity:** Measures atmospheric humidity levels.
   - **Light Intensity:** Monitors the amount of light reaching the plants.

2. **Real-Time Data Upload**:
   - Integrates with **ThingSpeak** to upload and visualize data remotely.

3. **Threshold Alerts**:
   - Warns about out-of-range values for each parameter, helping users take timely corrective actions.

---

## Components

### Hardware

1. **Microcontroller**:
   - Arduino Uno
2. **Sensors**:
   - **Capacitive Soil Moisture Sensor** (Analog)
   - **Gravity pH Sensor Kit** (Analog)
   - **Gravity EC Sensor Kit** (Analog)
   - **DS18B20 Digital Temperature Sensor** (1-Wire)
   - **DHT22 Temperature and Humidity Sensor** (Digital)
   - **Photoresistor (LDR)** with 10kΩ Resistor (Analog)
3. **Wi-Fi Module** (Optional):
   - ESP8266 or ESP32 for data upload.

### Software

1. **Arduino IDE**: For code development and uploading.
2. **VS Code**: For code version control and advanced editing (optional).
3. **ThingSpeak**: For real-time data visualization.
4. **GitHub**: For version control and collaboration.

---

## Installation and Setup

### Hardware Setup
1. **Connect Sensors**:
   - Soil Moisture Sensor: Connect to **A0**.
   - pH Sensor: Connect to **A1**.
   - EC Sensor: Connect to **A2**.
   - DS18B20: Connect to **Pin 2** (1-Wire communication).
   - DHT22: Connect to **Pin 3**.
   - LDR: Connect to **A3** with a 10kΩ resistor voltage divider.
2. **Power**:
   - Ensure all sensors are powered with 3.3V or 5V as required.
3. **Optional Wi-Fi Module**:
   - Connect the ESP8266 or ESP32 module for wireless data upload.

### Software Setup
1. **Install Libraries**:
   - Install the following libraries via the Arduino IDE Library Manager:
     - ThingSpeak
     - ESP8266WiFi (if using ESP8266)
     - OneWire
     - DallasTemperature
     - DHT sensor library
2. **Upload Code**:
   - Configure Wi-Fi credentials and ThingSpeak channel details in the code.
   - Upload the code to the Arduino Uno using the Arduino IDE.
3. **Verify Data**:
   - Open the Serial Monitor to check sensor readings and confirm functionality.
   - Verify data uploads to ThingSpeak.

---

## Usage

1. Deploy the device in your garden, ensuring sensors are placed appropriately:
   - Insert the soil moisture, pH, and EC sensors into the soil.
   - Place the temperature and humidity sensor in an exposed location for accurate readings.
   - Position the light sensor to face the primary light source.
2. Monitor the garden conditions via ThingSpeak.
3. Use threshold alerts to take corrective actions, such as watering, adding nutrients, or adjusting light exposure.

---

## Repository Structure

```
Gardinfo/
├── Gardinfo.ino         # Main Arduino code
├── README.md            # Project documentation
├── LICENSE              # Project license (optional)
├── .gitignore           # Git ignore file
└── libraries/           # Custom libraries if needed
```

---

## Future Enhancements

1. Add more sensors, such as CO2 or wind speed sensors.
2. Implement Bluetooth for local data access.
3. Create a mobile app for easier data visualization and alerts.
4. Optimize the code for power efficiency.

---

## Contributing

Contributions are welcome! Please fork the repository and submit a pull request with your proposed changes.

---

## License

This project is licensed under the MIT License. See the LICENSE file for more details.

---

## Acknowledgments

Special thanks to the open-source community for providing the libraries and resources used in this project.

