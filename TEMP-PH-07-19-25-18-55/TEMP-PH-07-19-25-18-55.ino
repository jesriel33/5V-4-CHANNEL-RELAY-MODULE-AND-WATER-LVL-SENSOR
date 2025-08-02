#include <OneWire.h>
#include <DallasTemperature.h>

// --- DS18B20 Setup ---
#define ONE_WIRE_BUS 7
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempSensor(&oneWire);

// --- pH Sensor Setup ---
#define PH_PIN A0
float calibration_offset = 6.8 - 5.63; //using pH 6.68 (/*-+) pH measured shown ← adjust this using buffer solution
float voltage, pH, analogAvg;
float waterTempC = 25.0;

void setup() {
  Serial.begin(9600);
  tempSensor.begin();
}

void loop() {
  // --- Read Temperature ---
  tempSensor.requestTemperatures();
  waterTempC = tempSensor.getTempCByIndex(0);
  if (waterTempC == DEVICE_DISCONNECTED_C) {
    Serial.println("Error: DS18B20 not found! Using default temp.");
    waterTempC = 25.0;
  }

  // --- Read pH Sensor ---
  int buffer[10];
  for (int i = 0; i < 10; i++) {
    buffer[i] = analogRead(PH_PIN);
    delay(20);
  }

  // Sort buffer to remove outliers
  for (int i = 0; i < 9; i++) {
    for (int j = i + 1; j < 10; j++) {
      if (buffer[i] > buffer[j]) {
        int temp = buffer[i];
        buffer[i] = buffer[j];
        buffer[j] = temp;
      }
    }
  }

  long avgValue = 0;
  for (int i = 2; i < 8; i++) avgValue += buffer[i];
  analogAvg = avgValue / 6.0;

  voltage = analogAvg * 5.0 / 1024.0;

  // --- Calculate pH ---
  // Formula: pH = 7 + (2.5 - V) / 0.18
  pH = 7.0 + ((2.5 - voltage) / 0.18) + calibration_offset;

  // Optional: Basic Temperature Compensation
  float compensation = (waterTempC - 25.0) * 0.03;
  pH += compensation;

  // --- Output to Serial ---
  Serial.print("Temp: ");
  Serial.print(waterTempC, 1);
  Serial.print(" °C | Analog Avg: ");
  Serial.print(analogAvg, 2);
  Serial.print(" | Voltage: ");
  Serial.print(voltage, 3);
  Serial.print(" V | pH: ");
  Serial.print(pH, 2);
  Serial.print(" | Offset: ");
  Serial.println(calibration_offset, 2);

  delay(2000);
}
