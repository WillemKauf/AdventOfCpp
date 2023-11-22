/////////////////
//// std
/////////////////
#include <string>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2017 {

struct day_05 : public Advent_type {
  static constexpr int year    = 2017;
  static constexpr int date    = 5;
  const std::vector<int> input = read_lines<int>(year, date);

  std::string part_1() override {
    auto instructs = input;
    const auto n   = instructs.size();

    int numSteps = 0;

    for (int p = 0; p < n; ++numSteps) {
      p += instructs[p]++;
    }

    return std::to_string(numSteps);
  }

  std::string part_2() override {
    auto instructs = input;
    const auto n   = instructs.size();

    int numSteps = 0;

    for (int p = 0; p < n; ++numSteps) {
      if (instructs[p] >= 3) {
        p += instructs[p]--;
      } else {
        p += instructs[p]++;
      }
    }

    return std::to_string(numSteps);
  }
};

};  // namespace AOC2017
