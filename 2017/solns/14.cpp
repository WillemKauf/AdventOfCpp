/////////////////
//// std
/////////////////

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"
#include "10.cpp"

namespace AOC2017 {

struct day_14 : public Advent_type {
  static constexpr int year = 2017;
  static constexpr int date = 14;
  const std::string input   = read_lines<std::string>(year, date)[0];

  static constexpr int gridSize = 128;
  using Grid_type = std::array<std::array<int, gridSize>, gridSize>;


  Grid_type ParseInput() const {
    Grid_type grid;
    for(int j = 0; j < gridSize; ++j){
      const auto hashStr = input + "-" + std::to_string(j);
      const auto hash = AOC2017::RunHash
      grid[j];
    }
  }

  std::string part_1() override {
    
    return "";
  }

  std::string part_2() override {
    return "";
  }

};

};  // namespace AOC2017
