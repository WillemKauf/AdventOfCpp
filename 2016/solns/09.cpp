/////////////////
//// std
/////////////////
#include <algorithm>
#include <limits>
#include <numeric>
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

struct day_09 : public Advent_type {
  static constexpr int year = 2016;
  static constexpr int date = 9;
  const std::string input   = read_lines<std::string>(year, date)[0];

  std::string part_1() override {
    int length = 0;
    int p      = 0;
    while (p < input.size()) {
      const auto& c = input[p];
      if (c == '(') {
        const auto nextBracket = input.find_first_of(')', p);
        const auto bracketStr  = input.substr(p + 1, nextBracket - p - 1);
        const auto xPos        = bracketStr.find_first_of('x');
        const auto charCount   = std::stoi(bracketStr.substr(0, xPos));
        const auto repeatCount = std::stoi(bracketStr.substr(xPos + 1));
        p = nextBracket + charCount + 1;
        length += charCount * repeatCount;
      } else {
        ++p;
        ++length;
      }
    }
    return std::to_string(length);
  }

  uint64_t recurse(int& p, int currLength, int multiplier) {
    uint64_t length = 0;
    while (p < input.size() && currLength > 0) {
      const auto& c = input[p];
      if (c == '(') {
        const auto nextBracket = input.find_first_of(')', p);
        const auto bracketStr  = input.substr(p + 1, nextBracket - p - 1);
        const auto xPos        = bracketStr.find_first_of('x');
        const auto charCount   = std::stoi(bracketStr.substr(0, xPos));
        const auto repeatCount = std::stoi(bracketStr.substr(xPos + 1));
        const auto currP       = p;
        p                      = nextBracket + 1;
        length += recurse(p, charCount, multiplier * repeatCount);
        currLength -= (p - currP);
      } else {
        ++p;
        --currLength;
        length += multiplier;
      }
    }
    return length;
  }

  std::string part_2() override {
    int p = 0;
    return std::to_string(recurse(p, std::numeric_limits<int>::max(), 1));
  }
};

};  // namespace AOC2016
