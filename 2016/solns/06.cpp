/////////////////
//// std
/////////////////
#include <algorithm>
#include <bitset>
#include <limits>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

namespace AOC2016 {

struct day_06 : public Advent_type {
  static constexpr int year            = 2016;
  static constexpr int date            = 6;
  const std::vector<std::string> input = read_lines<std::string>(year, date);
  static constexpr int wordSize        = 8;

  std::string part_1() override {
    using Map_type                          = std::unordered_map<char, int>;
    std::array<Map_type, wordSize> mapArray = {};
    std::array<char, wordSize> maxChar      = {};

    for (const auto& str : input) {
      for (auto [index, c] : std::views::enumerate(str)) {
        auto& indexArray = mapArray[index];
        auto& cFreq      = ++indexArray[c];
        if (cFreq >= indexArray[maxChar[index]]) {
          maxChar[index] = c;
        }
      }
    }
    return std::string(maxChar.begin(), maxChar.end());
  }

  std::string part_2() override {
    using Map_type                          = std::unordered_map<char, int>;
    std::array<Map_type, wordSize> mapArray = {};
    for (const auto& str : input) {
      for (auto [index, c] : std::views::enumerate(str)) {
        ++mapArray[index][c];
      }
    }
    std::string minStr = "";
    for (const auto& map : mapArray) {
      auto minElement = *std::min_element(
          map.begin(), map.end(), [](const auto& a, const auto& b) { return a.second < b.second; });
      minStr += minElement.first;
    }
    return minStr;
  }
};

};  // namespace AOC2016
