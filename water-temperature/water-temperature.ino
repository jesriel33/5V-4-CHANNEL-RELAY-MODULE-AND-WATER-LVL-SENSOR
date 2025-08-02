/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-temperature-sensor
 */
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const int SENSOR_PIN = 2; // Arduino pin connected to DS18B20 sensor's DQ pin

OneWire oneWire(SENSOR_PIN);         // setup a oneWire instance
DallasTemperature tempSensor(&oneWire); // pass oneWire to DallasTemperature library

// LCD Setup
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Adjust address if needed


float tempCelsius;    // temperature in Celsius
float tempFahrenheit; // temperature in Fahrenheit

void setup()
{
  Serial.begin(9600); // initialize serial
  tempSensor.begin();    // initialize the sensor
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("EC Meter Ready");
  delay(1500);
  lcd.clear();
}

void loop()
{
  tempSensor.requestTemperatures();             // send the command to get temperatures
  tempCelsius = tempSensor.getTempCByIndex(0);  // read temperature in Celsius
  tempFahrenheit = tempCelsius * 9 / 5 + 32; // convert Celsius to Fahrenheit

  Serial.print("Temperature: ");
  Serial.print(tempCelsius);    // print the temperature in Celsius
  Serial.print("°C");
  Serial.print("  ~  ");        // separator between Celsius and Fahrenheit
  Serial.print(tempFahrenheit); // print the temperature in Fahrenheit
  Serial.println("°F");

  //LCD output
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(tempCelsius);
  lcd.print("°C");
  lcd.setCursor(0, 1);
  lcd.print("  ~  ");
  lcd.print(tempFahrenheit);
  lcd.print("°F");

  delay(500);
}
