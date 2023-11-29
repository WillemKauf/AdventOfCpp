/////////////////
//// std
/////////////////
#include <cstdint>
#include <limits>
#include <stdexcept>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

namespace AOC2015 {

struct day_25 : public Advent_type {
  static constexpr int year = 2015;
  static constexpr int date = 25;

  const std::vector<int> input = read_lines_vector_regex<int>(year, date, "(\\d+)")[0];

  const int& row = input[0];
  const int& col = input[1];

  static constexpr int firstCode = 20151125;
  static constexpr int mulFactor = 252533;
  static constexpr int modFactor = 33554393;

  std::string part_1() override { return "Unimplemented"; }

  std::string part_2() override { return DayTwentyFiveString; }
};

/*
   | 1   2   3   4   5   6
---+---+---+---+---+---+---+
 1 |  1   3   6  10  15  21
 2 |  2   5   9  14  20  27
 3 |  4   8  13  19  26
 4 |  7  12  18  25
 5 | 11  17  24
 6 | 16  23
 7 | 22
*/

};  // namespace AOC2015
