// ---- Includes (if using temp sensor like DS18B20) ----
#include <OneWire.h>
#include <DallasTemperature.h>

// ---- Pin Config ----
#define PH_PIN A0
#define ONE_WIRE_BUS 7  // digital pin for DS18B20

// ---- Temp Sensor Setup ----
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempSensors(&oneWire);

// ---- Polynomial Calibration Coefficients (from your data) ----
// pH = a*V^2 + b*V + c
const float a = -0.0802;
const float b = -5.1821;
const float c = 22.2123;

// ---- Helpers ----
const int samples = 20;
float pH_value;

void setup() {
  Serial.begin(9600);
  tempSensors.begin();  // Start temperature sensor
}

void loop() {
  // Read temperature
  tempSensors.requestTemperatures();
  float waterTempC = tempSensors.getTempCByIndex(0);
  if (waterTempC == DEVICE_DISCONNECTED_C) {
    Serial.println("DS18B20 sensor not detected. Using fallback 25.0°C");
    waterTempC = 25.0;
  }

  // Take multiple analog samples for stability
  long total = 0;
  for (int i = 0; i < samples; i++) {
    total += analogRead(PH_PIN);
    delay(10);
  }

  float avg_reading = total / (float)samples;
  float voltage = avg_reading * (5.0 / 1023.0);
  
  //Failsafe
  if (voltage < 0.1 || voltage > 4.9) {
    Serial.println("Warning: Sensor voltage out of range");
    pH_value = -1.0;  // invalid reading
  } else {
    // Calculate pH using polynomial equation
    pH_value = a * voltage * voltage + b * voltage + c;
  }

  // Display
  Serial.print("Temp: ");
  Serial.print(waterTempC);
  Serial.print(" °C | Analog Avg: ");
  Serial.print(avg_reading, 2);
  Serial.print(" | Voltage: ");
  Serial.print(voltage, 3);
  Serial.print(" V | pH: ");
  Serial.println(pH_value, 2);

  delay(2000);  // 2 seconds delay between readings
}
