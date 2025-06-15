#include "bike_logic.h"

int histogram[10] = {0};
long total = 0;
int count = 0;

void pcs(){
printColorStats(histogram,total,count);

}

void printColorStats(const int histogram[], long total, int count) {
  float average = count > 0 ? (float)total / count : 0.0;

  Serial.print("Average value: ");
  Serial.println(average, 2);
  Serial.println();
  Serial.println("| Range     | Count |");
  Serial.println("|-----------|--------|");

  for (int i = 0; i < 10; i++) {
    int rangeStart = i * 10;
    int rangeEnd = (i == 9) ? 100 : (rangeStart + 9);
    int binCount = histogram[i];

    // Format range: right-align start, left-align end
    char rangeBuf[12];
    snprintf(rangeBuf, sizeof(rangeBuf), "%3d-%-3d", rangeStart, rangeEnd);

    // Format count: right-align with 3 spaces
    char countBuf[6];
    snprintf(countBuf, sizeof(countBuf), "%3d", binCount);

    // Final row print
    Serial.print("| ");
    Serial.print(rangeBuf);
    Serial.print(" |   ");
    Serial.print(countBuf);
    Serial.println(" |");
  }
}

int detectColor(int value) {
  // Update average
  total += value;
  count++;

  // Update histogram (bins: 0–9, 10–19, ..., 90–100)
  int bin = value / 10;
  if (bin >= 10) bin = 10 - 1;
  histogram[bin]++;

  // Classify value
  return (value > red_above_this) ? 0 : 1;
}

float calculateKPH(float rpm) {
  float meters_per_minute = rpm * wheel_circumference;
  return (meters_per_minute * 60) / 1000;
}

