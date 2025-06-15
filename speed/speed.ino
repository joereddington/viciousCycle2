#include <Wire.h>
#include "bike_logic.h"
#include <LiquidCrystal_I2C.h>

const int ldrPin = A0;
const int segments_per_revolution = 8;
int lastValue = 0;
unsigned long changes = 0;
unsigned total_readings = 0;
unsigned long changesSinceLastTick = 0;
unsigned long lastCheckTime = 0;
float rps = 0.0;

const int output_tick_size=500;
unsigned long startTime = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2); // Change 0x27 if needed

void setup() {
  Serial.begin(9600);
  pinMode(ldrPin, INPUT);
  startTime = millis();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Hello");
}

void loop() {
  int currentValue = detectColor(analogRead(ldrPin));//need some sort of error handling for when the pin is out. 
  unsigned long currentTime = millis();
  total_readings++;

  if (currentValue != lastValue) {
    lastValue = currentValue;
    changes++;
    changesSinceLastTick++;
  }

  if (currentTime - lastCheckTime >= output_tick_size) {
    rps = (float)changesSinceLastTick / segments_per_revolution;
    float rpm = rps * 60.0*(1000/output_tick_size);
    float cadence = rpm / 4.3; //Why is this 4.3? 
    Serial.println(changesSinceLastTick);
    float kph = calculateKPH(rpm);

    double totalRevolutions = (double)changes / segments_per_revolution;
    float distance_km = (totalRevolutions * wheel_circumference) / 1000.0;

    float totalTimeHours = (currentTime - startTime) / 3600000.0;
    float averageKPH = (totalTimeHours > 0) ? distance_km / totalTimeHours : 0;
    float readings_per_change = total_readings/changes;

    lastCheckTime = currentTime;
    changesSinceLastTick = 0;
    Serial.print("Changesx: ");
    Serial.print(changes);
    Serial.print("Readings Per Change: ");
    Serial.print(readings_per_change);
    Serial.print("RPS: ");
    Serial.print(rps, 1);
    Serial.print("RPM: ");
    Serial.print(rpm, 1);
    Serial.print(" | Cadence: ");
    Serial.print(cadence, 1);
    Serial.print(" | KPH: ");
    Serial.print(kph, 2);
    Serial.print(" | Distance: ");
    Serial.print(distance_km, 3);
    Serial.print(" km | Avg KPH: ");
    Serial.println(averageKPH, 2);

    char line1[17]; // 16 chars + null terminator
    char line2[17];

    sprintf(line1, "CAD:%3d KPH:%5.1f", (int)cadence, kph);
    sprintf(line2, "Dist:%6.2fkm", distance_km);
    Serial.print("LINE1: '");
Serial.print(line1);
Serial.println("'");

char buf[20];
float f = 12.34;
sprintf(buf, "Float: %5.1f", f);
Serial.println(buf); // If this prints "Float: ?" or garbage — you’ve lost float support

    lcd.clear();  // clear entire screen
   
    // First row: RPM and KPH
lcd.setCursor(0, 0);
lcd.print("CAD:");
lcd.print(cadence, 0);     // no decimals
lcd.print(" KPH:");
lcd.print(kph, 1);     // 1 decimal

// Second row: Distance in km
lcd.setCursor(0, 1);
lcd.print("Dist:");
lcd.print(distance_km, 2);  // 2 decimals
lcd.print("km");

  }
  delay(1);
}
