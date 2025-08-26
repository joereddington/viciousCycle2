#ifndef BIKE_LOGIC_H
#define BIKE_LOGIC_H

#ifdef ARDUINO //this is so I can run tests on my mac
#include <Arduino.h>
#else
// Minimal Arduino type stubs for testing
#include <cstdint>
typedef unsigned char byte;
#endif

const float diameter_m = 0.67;//measured on Kat's tri bike
const float gear_ratio=3.34;//I worked this out by doing 100 crank revolutions and checking the number of main revolutions recorded
const float wheel_circumference = 3.1416 * diameter_m; // meters
const int red_above_this = 35;
const int segments_per_revolution = 60;
const float cadence_to_kph_scalar=wheel_circumference*gear_ratio*60/1000; //The 60 is to go from per minute to per hour the 1000 is to go from m to km
const int changes_per_ten_meters= (int) 10/(wheel_circumference/segments_per_revolution); //Used to work out km. Back of envelope is about 300 

int detectColor(int value);
int calculateKPH(int cadence);
float get_distance(long changes); 
//void run_startup_tests();
String padn(int value, int n);
#endif

