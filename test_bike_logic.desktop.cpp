#include <iostream>
#include <iomanip>
#include "speed/bike_logic.h"

// --------- Mock Arduino Serial ----------
struct {
  void print(const char* s) { std::cout << s; }
  void print(int v) { std::cout << v; }
  void print(float f, int precision = 2) {
    std::cout << std::fixed << std::setprecision(precision) << f;
  }
  void println(const char* s) { std::cout << s << "\n"; }
  void println(int v) { std::cout << v << "\n"; }
  void println(float f, int precision = 2) {
    std::cout << std::fixed << std::setprecision(precision) << f << "\n";
  }
  void println() { std::cout << "\n"; }
} Serial;

// --------- Include the actual logic ----------
#include "speed/bike_logic.cpp"

// --------- Main test ----------
int main() {
  int testHistogram[10] = {2, 5, 3, 0, 1, 4, 0, 0, 0, 1};
  long testTotal = 785;
  int testCount = 20;

  printColorStats(testHistogram, testTotal, testCount);
  return 0;
}

