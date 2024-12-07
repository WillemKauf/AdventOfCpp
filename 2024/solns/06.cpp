/////////////////
//// std
/////////////////
#include <complex>
#include <cstddef>
#include <cstdint>
#include <numeric>
#include <queue>
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
#include "strings.h"

namespace AOC2024 {

struct day_06 : public Advent_type {
  static constexpr int year = 2024;
  static constexpr int date = 6;

  using Grid_type = std::vector<std::vector<char>>;
  using Set_type  = std::unordered_set<uint32_t>;

  struct Input_type {
    std::vector<std::vector<char>> grid;
    size_t start_x;
    size_t start_y;
  };

  Input_type ParseInput() const {
    Grid_type grid = read_lines_vector(year, date);
    size_t start_x;
    size_t start_y;
    for (size_t j = 0; j < grid.size(); ++j) {
      for (size_t i = 0; i < grid[j].size(); ++i) {
        if (grid[j][i] == '^') {
          start_x = i;
          start_y = j;
          break;
        }
      }
    }

    return Input_type{std::move(grid), start_x, start_y};
  }

  Input_type input      = ParseInput();
  Grid_type& grid       = input.grid;
  const size_t& start_x = input.start_x;
  const size_t& start_y = input.start_y;
  const size_t m        = grid.size();
  const size_t n        = grid[0].size();

  Set_type seen;

  template <bool part_two = false>
  bool path_find() {
    size_t x                                     = start_x;
    size_t y                                     = start_y;
    std::complex<int> dd                         = {0, -1};
    static constexpr std::complex<int> imag_unit = {0, 1};

    // Arbitrary upper bound for loop detection.
    static constexpr int limit = 1e4;
    int num_steps              = 0;
    while (Grid::InBounds(x, y, m, n)) {
      if constexpr (part_two) {
        if (++num_steps == limit) {
          break;
        }
      } else {
        seen.insert(Grid::HashIndex<uint32_t>(x, y, n));
      }
      const auto nx = x + dd.real();
      const auto ny = y + dd.imag();
      if (Grid::InBounds(nx, ny, m, n) && grid[ny][nx] == '#') {
        dd *= imag_unit;
        continue;
      }
      x = nx;
      y = ny;
    }

    return num_steps == limit;
  }

  std::string part_1() override {
    path_find();
    return std::to_string(seen.size());
  }

  std::string part_2() override {
    return std::to_string(std::accumulate(seen.begin(), seen.end(), uint32_t{0},
                                          [this](uint32_t s, const uint32_t hash) {
                                            auto [i, j] = Grid::HashToPosition<size_t>(hash, n);
                                            grid[j][i]  = '#';
                                            s += path_find<true>();
                                            grid[j][i] = '.';
                                            return s;
                                          }));
  }
};

}  // namespace AOC2024
