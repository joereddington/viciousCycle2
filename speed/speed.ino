#include <Wire.h>
#include "bike_logic.h"
#include <LiquidCrystal_I2C.h>

const int ldrPin = A0;
const int segments_per_revolution = 8;
int lastValue = 0;

const int output_tick_size = 500;
LiquidCrystal_I2C lcd(0x27, 16, 2); // Change 0x27 if needed

void updateLCD(float cadence, float kph, float distance_km) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CAD:");
  lcd.print(cadence, 0);
  lcd.print(" KPH:");
  lcd.print(kph, 1);

  lcd.setCursor(0, 1);
  lcd.print("Dist:");
  lcd.print(distance_km, 2);
  lcd.print("km");
}

void updateSerial(unsigned long changes,
                  float readings_per_change,
                  float rps,
                  float rpm,
                  float cadence,
                  float kph,
                  float distance_km,
                  float averageKPH) {
  Serial.print("| Changes: ");
  Serial.print(changes);
  Serial.print("| Readings Per Change: ");
  Serial.print(readings_per_change);
  Serial.print("| RPS: ");
  Serial.print(rps, 1);
  Serial.print(" | RPM: ");
  Serial.print(rpm, 1);
  Serial.print(" | Cadence: ");
  Serial.print(cadence, 1);
  Serial.print(" | KPH: ");
  Serial.print(kph, 2);
  Serial.print(" | Distance: ");
  Serial.print(distance_km, 3);
  Serial.print(" km | Avg KPH: ");
  Serial.println(averageKPH, 2);
}

void setup() {
  Serial.begin(9600);
  pinMode(ldrPin, INPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Hello");
}

void loop() {
  static unsigned long startTime = millis();
  static unsigned long changes = 0;
  static unsigned total_readings = 0;
  static unsigned long changesSinceLastTick = 0;
  static unsigned long lastCheckTime = 0;
  static float rps = 0.0;

  int currentValue = detectColor(analogRead(ldrPin));
  unsigned long currentTime = millis();
  total_readings++;

  if (currentValue != lastValue) {
    lastValue = currentValue;
    changes++;
    changesSinceLastTick++;
  }

  if (currentTime - lastCheckTime >= output_tick_size) {
    rps = (float)changesSinceLastTick / segments_per_revolution;
    float rpm = rps * 60.0 * (1000 / output_tick_size);
    float cadence = rpm / 4.3;
    float kph = calculateKPH(rpm);

    double totalRevolutions = (double)changes / segments_per_revolution;
    float distance_km = (totalRevolutions * wheel_circumference) / 1000.0;

    float totalTimeHours = (currentTime - startTime) / 3600000.0;
    float averageKPH = (totalTimeHours > 0) ? distance_km / totalTimeHours : 0;
    float readings_per_change = (changes > 0) ? (float)total_readings / changes : 0;

    lastCheckTime = currentTime;
    changesSinceLastTick = 0;

    updateSerial(changes, readings_per_change, rps, rpm, cadence, kph, distance_km, averageKPH);
    updateLCD(cadence, kph, distance_km);
  }

  delay(1);
}

