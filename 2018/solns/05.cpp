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
#include "strings.h"

namespace AOC2018 {

struct day_05 : public Advent_type {
  static constexpr int year = 2018;
  static constexpr int date = 5;
  const std::string input   = read_lines<std::string>(year, date)[0];

  static constexpr int charDiff = 32;

  std::string ReactPolymer(const auto& polymer) const {
    std::string st;
    for (const auto& c : polymer) {
      if (st.empty()) {
        st.push_back(c);
      } else {
        if (std::abs(st.back() - c) == charDiff) {
          st.pop_back();
        } else {
          st.push_back(c);
        }
      }
    }
    return st;
  }

  std::string part_1() override { return std::to_string(ReactPolymer(input).size()); }

  std::string part_2() override {
    int minLength                     = std::numeric_limits<int>::max();
    static const std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
    for (const auto& c : alphabet) {
      auto polymer = input;
      Strings::EraseAll(polymer, c);
      Strings::EraseAll(polymer, c - charDiff);
      minLength = std::min(minLength, static_cast<int>(ReactPolymer(polymer).size()));
    }
    return std::to_string(minLength);
  }
};

};  // namespace AOC2018
