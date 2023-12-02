/////////////////
//// std
/////////////////

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2018 {

struct day_06 : public Advent_type {
  static constexpr int year = 2018;
  static constexpr int date = 6;
  const std::vector<std::vector<int>> input = read_lines_vector_regex<int>(year, date, "(\\d+)");

  std::string part_1() override {
    return "Unimplemented";
  }

  std::string part_2() override {
    return "Unimplemented";
  }

};

};  // namespace AOC2018
