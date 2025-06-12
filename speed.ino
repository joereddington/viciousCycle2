#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int ldrPin = A0;
const int red_above_this = 35;
const int segments_per_revolution = 8;
int lastValue = 0;
unsigned long changes = 0;
unsigned long changesInLastSecond = 0;
int ldrValue = 0;
unsigned long lastCheckTime = 0;
float rps = 0.0;

const float diameter_m = 0.67;
const float wheel_circumference = 3.1416 * diameter_m; // meters
const int tick_size=500;
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

int detectColor(int value) {
  return (value > red_above_this) ? 0 : 1;
}

float calculateKPH(float rpm) {
  float meters_per_minute = rpm * wheel_circumference;
  return (meters_per_minute * 60) / 1000;
}

void loop() {
  ldrValue = analogRead(ldrPin);
  int currentValue = detectColor(ldrValue);//need some sort of error handling for when the pin is out. 
  unsigned long currentTime = millis();

  if (currentValue != lastValue) {
    lastValue = currentValue;
    changes++;
    changesInLastSecond++;
  }

  if (currentTime - lastCheckTime >= tick_size) {
    rps = (float)changesInLastSecond / segments_per_revolution;
    float rpm = rps * 60.0*(1000/tick_size);//TODO - this should depend in the update tick size
    float cadence = rpm / 4.3;
    Serial.println(changesInLastSecond);
    float kph = calculateKPH(rpm);

    double totalRevolutions = (double)changes / segments_per_revolution;
    float distance_km = (totalRevolutions * wheel_circumference) / 1000.0;

    float totalTimeHours = (currentTime - startTime) / 3600000.0;
    float averageKPH = (totalTimeHours > 0) ? distance_km / totalTimeHours : 0;

    lastCheckTime = currentTime;
    changesInLastSecond = 0;
 Serial.print("Changesx: ");
    Serial.print(changes);
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

  delay(10);
}
