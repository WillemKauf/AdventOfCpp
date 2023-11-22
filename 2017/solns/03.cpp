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

/*
37 36  35  34  33  32 31
38 17  16  15  14  13 30
39 18   5   4   3  12 29
40 19   6   1   2  11 28
41 20   7   8   9  10 27
42 21  22  23  24  25 26
43 44  45  46  47  48 49 50
*/

struct day_03 : public Advent_type {
  static constexpr int year = 2017;
  static constexpr int date = 3;
  const int input           = std::stoi(read_lines<std::string>(year, date)[0]);

  std::string part_1() override {
    const auto computeSector = [](int i) { return (2 * i + 1) * (2 * i + 1); };
    int mag                  = {};
    int sectorNum            = 0;
    for (; sectorNum < 1e6; ++sectorNum) {
      const auto v = computeSector(sectorNum);
      if (v >= input) {
        const auto distOne = input - v;
        if (distOne > 0) {
          mag = distOne;
        } else {
          --sectorNum;
          mag = input - computeSector(sectorNum);
        }
        break;
      }
    }
    int x = sectorNum;
    int y = -sectorNum;
    // Move to right one.
    ++x;
    --mag;
    if (mag == 0) {
      return std::to_string(std::abs(x) + std::abs(y));
    }

    const auto sectorSize = 2 * (sectorNum + 1);

    // Move across right border of sector.
    for (int j = 0; mag > 0 && j < sectorSize; ++j, --mag) {
      ++y;
    }

    if (mag == 0) {
      return std::to_string(std::abs(x) + std::abs(y));
    }

    // Move across top border of sector.
    for (int i = 0; mag > 0 && i < sectorSize; ++i, --mag) {
      --x;
    }

    if (mag == 0) {
      return std::to_string(std::abs(x) + std::abs(y));
    }

    // Move across left border of sector.
    for (int j = 0; mag > 0 && j < sectorSize; ++j, --mag) {
      --y;
    }

    if (mag == 0) {
      return std::to_string(std::abs(x) + std::abs(y));
    }

    // Move across bottom border of sector.
    for (int i = 0; mag > 0 && i < sectorSize; ++i, --mag) {
      ++x;
    }

    if (mag == 0) {
      return std::to_string(std::abs(x) + std::abs(y));
    }

    std::unreachable();
  }

  std::string part_2() override {
    static constexpr auto ddir = Grid::GetAllCardinalDirs<Grid::GridDirection::WithDiagonals>();
    std::unordered_map<int, int> grid;
    grid[0]       = 1;
    int i         = 1;
    int j         = 0;
    int sectorNum = 0;
    int di        = 0;
    int dj        = 1;
    int dx = 2 * (sectorNum + 1);
    int dy = 2 * (sectorNum + 1)-1;
    while (true) {
      int value = {};
      for (const auto& dd : ddir) {
        const auto ii   = i + dd[0];
        const auto jj   = j + dd[1];
        const auto hash = ii + jj * 347993;
        if (auto gridIt = grid.find(hash); gridIt != grid.end()) {
          value += gridIt->second;
        }
      }
      //std::cout << i << " " << j << " " << value << " " << sectorSize << '\n';
      break;
    }
    return "";
  }
};

};  // namespace AOC2017
