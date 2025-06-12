#ifndef BIKE_LOGIC_H
#define BIKE_LOGIC_H

#ifdef ARDUINO //this is so I can run tests on my mac
#include <Arduino.h>
#else
// Minimal Arduino type stubs for testing
#include <cstdint>
typedef unsigned char byte;
#endif

const float diameter_m = 0.67;
const float wheel_circumference = 3.1416 * diameter_m; // meters
const int red_above_this = 35;


extern int histogram[10];
extern long total;
extern int count;



void printColorStats(const int histogram[], long total, int count);
int detectColor(int value);
float calculateKPH(float rpm);


#endif

