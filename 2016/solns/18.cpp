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

struct day_18 : public Advent_type {
  static constexpr int year = 2016;
  static constexpr int date = 18;
  const std::string input   = read_lines<std::string>(year, date)[0];

  int getNumTraps(int numRows) {
    auto currRow                = input;
    static const auto inputSize = input.size();
    const auto getTile          = [](const auto& window) {
      const auto isTrap = (window[0] == '^' && window[1] == '^' && window[2] == '.') ||
                          (window[0] == '.' && window[1] == '^' && window[2] == '^') ||
                          (window[0] == '^' && window[1] == '.' && window[2] == '.') ||
                          (window[0] == '.' && window[1] == '.' && window[2] == '^');
      return isTrap ? '^' : '.';
    };

    int numSafeTiles = 0;
    for (int i = 0; i < numRows; ++i) {
      numSafeTiles += std::count(currRow.begin(), currRow.end(), '.');
      currRow.push_back('.');
      currRow.insert(currRow.begin(), '.');
      std::string newRow;
      newRow.reserve(inputSize);
      for (auto window : std::views::slide(currRow, 3)) {
        newRow.push_back(getTile(window));
      }
      currRow = newRow;
    }
    return numSafeTiles;
  }

  std::string part_1() override {
    static constexpr int numRows = 40;
    return std::to_string(getNumTraps(numRows));
  }

  std::string part_2() override {
    static constexpr int numRows = 400000;
    return std::to_string(getNumTraps(numRows));
  }
};

};  // namespace AOC2016
