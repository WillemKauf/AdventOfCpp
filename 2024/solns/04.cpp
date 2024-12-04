/////////////////
//// std
/////////////////
#include <cstddef>
#include <cstdint>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"
#include "data_structure.h"
#include "grid.h"

namespace AOC2024 {

struct day_04 : public Advent_type {
  static constexpr int year = 2024;
  static constexpr int date = 4;

  const std::vector<std::vector<char>> input = read_lines_vector(year, date);

  std::string part_1() override {
    static constexpr auto dirs = Grid::GetAllCardinalDirs<Grid::GridDirection::WithDiagonals>();
    static const std::string xmas_str = "XMAS";
    uint32_t s                        = 0;
    for (size_t j = 0; j < input.size(); ++j) {
      for (size_t i = 0; i < input[j].size(); ++i) {
        for (const auto& dd : dirs) {
          size_t p = 0;
          for (auto [ii, jj] = std::tuple{i, j};
               Grid::InBounds(ii, jj, input.size(), input[jj].size()) && p < xmas_str.size() &&
               input[jj][ii] == xmas_str[p];
               ii += dd[0], jj += dd[1], ++p) {
          }

          if (p == xmas_str.size()) {
            ++s;
          }
        }
      }
    }
    return std::to_string(s);
  }

  std::string part_2() override {
    static constexpr auto dirs = Grid::GetAllCardinalDirs<Grid::GridDirection::WithDiagonals>();
    uint32_t s                 = 0;
    for (size_t j = 0; j < input.size(); ++j) {
      for (size_t i = 0; i < input[j].size(); ++i) {
        if (input[j][i] == 'A') {
          std::unordered_map<size_t, char> mp = {};
          for (const auto& dd : dirs) {
            if (std::abs(dd[0]) + std::abs(dd[1]) != 2) {
              // Only use diagonals.
              continue;
            }
            const size_t ii = i + dd[0];
            const size_t jj = j + dd[1];
            if (Grid::InBounds(ii, jj, input.size(), input[jj].size())) {
              const auto c = input[jj][ii];
              if (c == 'M' || c == 'S') {
                const int p = dd[0] + dd[1] * 2;
                mp[p]       = c;
              }
            }
          }
          if (mp.size() == 4 && (mp[-3] != mp[3]) && (mp[-1] != mp[1])) {
            ++s;
          }
        }
      }
    }
    return std::to_string(s);
  }
};

}  // namespace AOC2024
