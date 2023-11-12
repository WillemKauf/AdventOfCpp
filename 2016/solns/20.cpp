/////////////////
//// std
/////////////////

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2016 {

struct day_20 : public Advent_type {
  static constexpr int year = 2016;
  static constexpr int date = 20;
  const std::vector<std::vector<uint32_t>> input = read_lines_vector_regex<uint32_t>(year, date, "(\\d+)-(\\d+)");

  std::string part_1() override {
    
    return "";
  }

  std::string part_2() override {
    return "";
  }

};

};  // namespace AOC2016
