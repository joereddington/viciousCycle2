#include "bike_logic.h"

int detectColor(int value) {
  // Classify value
  return (value > red_above_this) ? 0 : 1;
}

int calculateKPH(int cadence) {
  if (cadence < 0){
    return 0;
  }
  return (int) cadence * cadence_to_kph_scalar;
}

float get_distance(long changes){
  if (changes < 0){
    return 0;
  }
  return  (float) changes/(changes_per_ten_meters*100);
}

//TODO - work out what the error messages are when you push to the Leonardo 

String padn(int value, int n) {
  String s = String(value);
  while (s.length() < n) {
    s = " " + s;  // left-pad with spaces
  }
  return s;
}


