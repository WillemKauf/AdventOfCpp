/////////////////
//// std
/////////////////
#include <utility>
#include <vector>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

struct day_01 : public Advent_type {
  static constexpr int year     = 2015;
  static constexpr int date     = 1;
  const std::vector<char> input = read_single_line(year, date);

  std::string part_1() override {
    return std::to_string(std::count(input.begin(), input.end(), '(') -
                          std::count(input.begin(), input.end(), ')'));
  }

  std::string part_2() override {
    int lvl = 0;
    for (std::size_t i = 0; const auto& c : input) {
      lvl += (c == '(') * 1 + (c == ')') * (-1);
      if (lvl == -1) {
        return std::to_string(i + 1);
      }
      ++i;
    }
    std::unreachable();
  }
};
