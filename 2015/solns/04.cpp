/////////////////
//// std
/////////////////
#include <unordered_set>
#include <utility>

/////////////////
//// local
/////////////////
#include "../common/common.h"
#include "../common/advent_base.h"
#include "../common/read_input.h"

namespace AOC2015 {

struct day_04 : public Advent_type {
  static constexpr int year = 2015;
  static constexpr int date = 4;
  const std::string input   = read_lines<std::string>(year, date)[0];
  int startingIndex         = {};

  auto getIndex(int numZeros, int startIndex = 0) {
    const auto compareString = std::string(numZeros, '0');
    for (int i = startIndex; true; ++i) {
      const auto toHash  = input + std::to_string(i);
      const auto hashStr = Hash::GetMD5Hash(toHash);
      if (hashStr.substr(0, numZeros) == compareString) {
        return std::make_pair(i, std::to_string(i));
      }
    }
    std::unreachable();
  }

  std::string part_1() override {
    auto result   = getIndex(5);
    startingIndex = result.first;
    return result.second;
  }

  std::string part_2() override { return getIndex(6, startingIndex).second; }
};

};  // namespace AOC2015
