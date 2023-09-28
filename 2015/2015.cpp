/////////////////
//// local
/////////////////
#include "2015.h"

/////////////////
//// std
/////////////////
#include <chrono>
#include <iostream>

int main() {
  double t = 0.0;
  for (int i = 1; i <= 25; ++i) {
    i        = 22;
    auto day = MakeDay(i);
    if (!day) {
      std::cout << "Day #" << i << " not implemented.\n";
      continue;
    }
    const auto start = std::chrono::system_clock::now();
    const auto res   = day->run_day();
    const auto end   = std::chrono::system_clock::now();
    const auto dt    = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Day #" << i << ": Part 1: " << res.first << ", Part 2: " << res.second << " | "
              << dt << "μs\n";
    t += dt;
    break;
  }
  std::cout << "-------------------------------------\n";
  std::cout << "Total time: " << t << "μs\n";
  return 0;
}
