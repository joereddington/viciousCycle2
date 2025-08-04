#include "bike_logic.h"

int detectColor(int value) {
  // Classify value
  return (value > red_above_this) ? 0 : 1;
}

float calculateKPH(int cadence) {
  return cadence * cadence_to_kph_scalar;
}

float get_distance(long changes){
  return  (float) changes/(changes_per_ten_meters*100);
}


