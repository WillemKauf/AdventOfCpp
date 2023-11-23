/////////////////
//// std
/////////////////

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2017 {

struct day_09 : public Advent_type {
  static constexpr int year     = 2017;
  static constexpr int date     = 9;
  const std::vector<char> input = read_single_line(year, date);

  std::string part_1() override {
    int ans        = 0;
    int depth      = 0;
    bool inGarbage = false;
    const auto n   = input.size();
    for (int p = 0; p < n; ++p) {
      const auto& c = input[p];
      switch (c) {
        case '!':
          ++p;
          break;
        case '<':
          inGarbage = true;
          break;
        case '>':
          inGarbage = false;
          break;
        case '{':
          depth += static_cast<int>(!inGarbage);
          break;
        case '}':
          ans += (inGarbage) ? 0 : depth--;
          break;
        default:
          break;
      }
    }
    return std::to_string(ans);
  }

  std::string part_2() override {
    int ans        = 0;
    bool inGarbage = false;
    const auto n   = input.size();
    for (int p = 0; p < n; ++p) {
      const auto& c = input[p];
      switch (c) {
        case '!':
          ++p;
          break;
        case '<':
          ans += static_cast<int>(inGarbage);
          inGarbage = true;
          break;
        case '>':
          inGarbage = false;
          break;
        default:
          ans += static_cast<int>(inGarbage);
          break;
      }
    }
    return std::to_string(ans);
  }
};

};  // namespace AOC2017
