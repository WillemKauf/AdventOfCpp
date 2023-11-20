/////////////////
//// std
/////////////////
#include<numeric>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2017 {

struct day_01 : public Advent_type {
  static constexpr int year = 2017;
  static constexpr int date = 1;
  const std::string input   = read_lines<std::string>(year, date)[0];

  std::string part_1() override {
    const auto n = input.size();
    int i        = 0;
    return std::to_string(
        std::accumulate(input.begin(), input.end(), 0, [&](int sm, const auto& v) {
          auto newSm = sm + ((v == input[(i + 1) % n]) ? static_cast<int>(v - '0') : 0);
          ++i;
          return newSm;
        }));
  }

  std::string part_2() override {
    const auto n     = input.size();
    int i            = 0;
    const auto halfN = n / 2;
    return std::to_string(
        std::accumulate(input.begin(), input.end(), 0, [&](int sm, const auto& v) {
          auto newSm = sm + ((v == input[(i + halfN) % n]) ? static_cast<int>(v - '0') : 0);
          ++i;
          return newSm;
        }));
  }
};

};  // namespace AOC2017
