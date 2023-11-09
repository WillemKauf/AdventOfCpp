/////////////////
//// std
/////////////////
#include <algorithm>
#include <cstdint>
#include <limits>
#include <numeric>
#include <queue>
#include <unordered_map>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

namespace AOC2015 {

struct day_10 : public Advent_type {
  static constexpr int year = 2015;
  static constexpr int date = 10;

  const std::string input = read_lines_string(year, date)[0];
  std::string initialResult;

  std::string applyProcess(int n, std::string str) {
    for (int i = 0; i < n; ++i) {
      std::string newStr;
      auto c        = str[0];
      int currCount = 1;
      for (int j = 1; j < str.size(); ++j) {
        auto cc = str[j];
        if (c == cc) {
          ++currCount;
        } else {
          newStr += std::to_string(currCount) + c;
          currCount = 1;
          c         = cc;
        }
      }
      newStr += std::to_string(currCount) + c;
      str = newStr;
    }
    return str;
  }

  std::string part_1() override {
    initialResult = applyProcess(40, input);
    return std::to_string(initialResult.size());
  }

  std::string part_2() override { return std::to_string(applyProcess(10, initialResult).size()); }
};

};  // namespace AOC2015
