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

float calculateKPH(float cadence) {
  float ratio=2.6; //todo make this a global
  float meters_per_minute = cadence * ratio * wheel_circumference;
  return (meters_per_minute * 60) / 1000;
}

float get_distance(long changes){
  if(changes<0){
    Serial.print("changes:"); Serial.print(changes);
    Serial.println("RETURNING BECAUSE CHANGES IS LESS THAN ZERO");
    return 0;
  }
  float ratio=2.6; //todo make this a global
  float total_revolutions = (float)changes / segments_per_revolution;
  float crank_revolutions= (float) total_revolutions/3.34;//todo why is this not using the main constant
  float km_traveled=(crank_revolutions*ratio)/1000*wheel_circumference;
  Serial.print("changes:"); Serial.print(changes);
  Serial.print("total revolutions:"); Serial.print(total_revolutions);
  Serial.print("crank_revolutions:"); Serial.print(crank_revolutions);
  Serial.print("km_traveled:"); Serial.print(km_traveled);
  Serial.println("");
  return km_traveled;
}


