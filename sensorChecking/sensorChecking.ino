#include <Wire.h>
//TODO - this needs display max and min
const int ldrPin = A0; // IR sensor pin
const int red_above_this = 85;

int lastValue = 0;
unsigned long lastPrintTime = 0;
unsigned int changes = 0;

#define MAX_READINGS 500
int readings[MAX_READINGS];
int numReadings = 0;

// Aggregated stats per state (0 and 1)
unsigned long totalCount[2] = {0, 0};
unsigned long totalRange[2] = {0, 0};
unsigned long totalMean[2] = {0, 0};
unsigned long totalStdev[2] = {0, 0};
unsigned long totalLength[2] = {0, 0}; // number of readings per state

void setup() {
  Serial.begin(9600);
  pinMode(ldrPin, INPUT);
  Serial.print("Hello");
}

void loop() {
  int sensor_read = analogRead(ldrPin);
  int currentValue = (sensor_read > red_above_this) ? 0 : 1;

  if (currentValue != lastValue) {
    // Only process if we have readings
    if (numReadings > 0) {
      int highest = readings[0];
      int lowest = readings[0];
      long sum = 0;

      for (int i = 0; i < numReadings; i++) {
        if (readings[i] > highest) highest = readings[i];
        if (readings[i] < lowest) lowest = readings[i];
        sum += readings[i];
      }

      float mean = (float)sum / numReadings;
      float sq_diff_sum = 0;
      for (int i = 0; i < numReadings; i++) {
        sq_diff_sum += pow(readings[i] - mean, 2);
      }
      float stdev = sqrt(sq_diff_sum / numReadings);

      //Serial.print("We had ");
      //Serial.print(numReadings);
      //Serial.print(" readings before change, range: ");
      //Serial.print(highest - lowest);
      //Serial.print(", mean: ");
      //Serial.print(mean);
      //Serial.print(", stdev: ");
      //Serial.println(stdev);

      // Store in aggregates
      int idx = lastValue;
      totalCount[idx]++;
      totalRange[idx] += (highest - lowest);
      totalMean[idx] += mean;
      totalStdev[idx] += stdev;
      totalLength[idx] += numReadings;
    }

    // Reset for new state
    numReadings = 0;
    readings[numReadings++] = sensor_read;
    lastValue = currentValue;
    changes++;
  } else {
    // No change, add reading to current state
    if (numReadings < MAX_READINGS) {
      readings[numReadings++] = sensor_read;
    }
  }

  // Every 10 seconds, print summary
  if (millis() - lastPrintTime > 10000) {
    lastPrintTime = millis();
    Serial.println("---- 10s Summary ----");
    for (int i = 0; i <= 1; i++) {
      if (totalCount[i] > 0) {
        Serial.print("For value ");
        Serial.print(i);
        Serial.print(": avg range = ");
        Serial.print((float)totalRange[i] / totalCount[i]);
        Serial.print(", avg mean = ");
        Serial.print((float)totalMean[i] / totalCount[i]);
        Serial.print(", avg stdev = ");
        Serial.print((float)totalStdev[i] / totalCount[i]);
        Serial.print(", avg length = ");
        Serial.println((float)totalLength[i] / totalCount[i]);
      }
    }
  }
}

