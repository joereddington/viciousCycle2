#ifndef BIKE_LOGIC_H
#define BIKE_LOGIC_H

#ifdef ARDUINO //this is so I can run tests on my mac
#include <Arduino.h>
#else
// Minimal Arduino type stubs for testing
#include <cstdint>
typedef unsigned char byte;
#endif

void printColorStats(const int histogram[], long total, int count);


#endif

