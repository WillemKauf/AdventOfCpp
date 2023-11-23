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

struct day_11 : public Advent_type {
  static constexpr int year = 2017;
  static constexpr int date = 11;
  const std::vector<std::string> input =
      read_lines_vector_regex<std::string>(year, date, "(\\w+)")[0];

  Grid::HexGridStats gridResults;

  std::string part_1() override {
    std::vector<Grid::HexGridDirections> hexDirectionsVec;
    for (const auto& v : input) {
      if (v == "n") {
        hexDirectionsVec.push_back(Grid::HexGridDirections::N);
      } else if (v == "s") {
        hexDirectionsVec.push_back(Grid::HexGridDirections::S);
      } else if (v == "nw") {
        hexDirectionsVec.push_back(Grid::HexGridDirections::NW);
      } else if (v == "ne") {
        hexDirectionsVec.push_back(Grid::HexGridDirections::NE);
      } else if (v == "sw") {
        hexDirectionsVec.push_back(Grid::HexGridDirections::SW);
      } else if (v == "se") {
        hexDirectionsVec.push_back(Grid::HexGridDirections::SE);
      }
    }
    gridResults = Grid::ParseHexGrid(hexDirectionsVec);
    return std::to_string(gridResults.distance);
  }

  std::string part_2() override { return std::to_string(gridResults.maxDistance); }
};

};  // namespace AOC2017
