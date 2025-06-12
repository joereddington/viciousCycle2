#include <iostream>
#include <iomanip>
#include "speed/bike_logic.h"
#include <cassert>


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


void resetStats() {
  for (int i = 0; i < 10; i++) histogram[i] = 0;
  total = 0;
  count = 0;
}

void test_detectColor() {
  resetStats();

  int val1 = detectColor(20);  // should return 1 (below threshold)
  int val2 = detectColor(40);  // should return 0 (above threshold)

  assert(val1 == 1);
  assert(val2 == 0);

  assert(count == 2);
  assert(total == 60);

  assert(histogram[2] == 1); // 20 falls into bin 2
  assert(histogram[4] == 1); // 40 falls into bin 4

  std::cout << "[PASS] test_detectColor()\n";
}

void test_calculateKPH() {
  float rpm = 60.0;
  float expected_kph = ((rpm * wheel_circumference) * 60.0f) / 1000.0f;

  float kph = calculateKPH(rpm);
  assert(std::abs(kph - expected_kph) < 0.01);

  std::cout << "[PASS] test_calculateKPH()\n";
}

// --------- Main test ----------

int main() {
  int testHistogram[10] = {2, 5, 3, 0, 1, 4, 0, 0, 0, 1};
  long testTotal = 785;
  int testCount = 20;

  printColorStats(testHistogram, testTotal, testCount);
  test_detectColor();
  test_calculateKPH();
  return 0;
}

