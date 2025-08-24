#include "bike_logic.h"

int detectColor(int value) {
  // Classify value
  return (value > red_above_this) ? 0 : 1;
}

float calculateKPH(int cadence) {
  return cadence * cadence_to_kph_scalar;
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
 // Serial.print("changes:"); Serial.print(changes);
  //Serial.print("total revolutions:"); Serial.print(total_revolutions);
  //Serial.print("crank_revolutions:"); Serial.print(crank_revolutions);
  //Serial.print("km_traveled:"); Serial.print(km_traveled);
  //Serial.println("");
  return km_traveled;
}


