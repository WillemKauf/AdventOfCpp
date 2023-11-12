/////////////////
//// openmp
/////////////////
#include <omp.h>

/////////////////
//// std
/////////////////
#include <cmath>
#include <limits>
#include <stdexcept>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

namespace AOC2015 {

struct day_20 : public Advent_type {
  static constexpr int year = 2015;
  static constexpr int date = 20;
  const int input           = std::stoi(read_lines<std::string>(year, date)[0]);

  std::string part_1() override {
    const auto numPresents = [](int houseNumber) {
      std::unordered_set<int> factors;
      for (int i = 1; i < static_cast<int>(std::sqrt(houseNumber)) + 1; ++i) {
        if (houseNumber % i == 0) {
          factors.insert(i);
          factors.insert(houseNumber / i);
        }
      }
      return std::accumulate(factors.begin(), factors.end(), 0) * 10;
    };

    // brute force through parallelization. I'm lazy.
    int res = std::numeric_limits<int>::max();
#pragma omp parallel for reduction(min : res)
    for (int i = 1; i < input / 10; ++i) {
      const auto n = numPresents(i);
      if (n >= input) {
        if (i < res) {
          res = std::min(res, i);
        }
      }
    }
    return std::to_string(res);
  }

  std::string part_2() override {
    const auto numPresents = [](int houseNumber) {
      std::unordered_set<int> factors;
      for (int i = 1; i <= 50; ++i) {
        if (houseNumber % i == 0) {
          factors.insert(houseNumber / i);
        }
      }
      return std::accumulate(factors.begin(), factors.end(), 0) * 11;
    };

    // brute force through parallelization. I'm lazy.
    int res = std::numeric_limits<int>::max();
#pragma omp parallel for reduction(min : res)
    for (int i = 1; i < input / 10; ++i) {
      const auto n = numPresents(i);
      if (n >= input) {
        if (i < res) {
          res = std::min(res, i);
        }
      }
    }
    return std::to_string(res);
  }
};

};  // namespace AOC2015
