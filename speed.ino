#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int ldrPin = A0;
const int red_above_this = 35;
const int segments_per_revolution = 8;
int lastValue = 0;
unsigned long changes = 0;
unsigned long changesSinceLastTick = 0;
int ldrValue = 0;
unsigned long lastCheckTime = 0;
float rps = 0.0;

const float diameter_m = 0.67;
const float wheel_circumference = 3.1416 * diameter_m; // meters

const int output_tick_size=500;
unsigned long startTime = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2); // Change 0x27 if needed


//For the history of colors 

const int HISTOGRAM_BINS = 10;
int histogram[HISTOGRAM_BINS] = {0};

long total = 0;
int count = 0;
//End history of colors

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
  // Update average
  total += value;
  count++;

  // Update histogram (bins: 0–9, 10–19, ..., 90–100)
  int bin = value / 10;
  if (bin >= HISTOGRAM_BINS) bin = HISTOGRAM_BINS - 1;
  histogram[bin]++;

  // Classify value
  return (value > red_above_this) ? 0 : 1;
}

void printColorStats() {
  float average = count > 0 ? (float)total / count : 0.0;

  Serial.print("Average value: ");
  Serial.println(average, 2);

  Serial.println();
  Serial.println("| Range     | Count |");
  Serial.println("|-----------|-------|");

  for (int i = 0; i < HISTOGRAM_BINS; i++) {
    int rangeStart = i * 10;
    int rangeEnd = (i == 9) ? 100 : (rangeStart + 9);

    // Format each row
    Serial.print("| ");
    if (rangeEnd < 100) Serial.print(" ");
    Serial.print(rangeStart);
    Serial.print("-");
    Serial.print(rangeEnd);
    Serial.print(" |   ");
    
    // Pad count to 3 digits
    if (histogram[i] < 10) Serial.print(" ");
    if (histogram[i] < 100) Serial.print(" ");
    Serial.print(histogram[i]);
    Serial.println(" |");
  }
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

    lastCheckTime = currentTime;
    changesSinceLastTick = 0;
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

    char line1[17]; // 16 chars + null terminator
    char line2[17];

    sprintf(line1, "CAD:%3d KPH:%5.1f", (int)cadence, kph);
    sprintf(line2, "Dist:%6.2fkm", distance_km);

    lcd.clear();  // clear entire screen
    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);

  }
  delay(10);
}
