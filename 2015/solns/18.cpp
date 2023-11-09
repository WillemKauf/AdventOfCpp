/////////////////
//// std
/////////////////
#include <limits>
#include <ranges>
#include <stdexcept>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2015 {

struct day_18 : public Advent_type {
  static constexpr int year = 2015;
  static constexpr int date = 18;

  const std::vector<std::vector<char>> input = read_lines_vector(year, date);
  const int n                                = input.size();
  static constexpr auto ddirs                = Grid::GetAllCardinalDirs<Grid::GridDirection::WithDiagonals>();

  template <typename T>
  auto lightFunction(char value, const T& neighbors) const {
    auto numNeighborsOn =
        std::count_if(neighbors.begin(), neighbors.end(), [](const auto& c) { return c == '#'; });
    if (value == '#') {
      return (numNeighborsOn == 2 || numNeighborsOn == 3) ? '#' : '.';
    } else {
      return (numNeighborsOn == 3) ? '#' : '.';
    }
  }

  template <bool fixCorners = false>
  int simulateSteps() const {
    static constexpr auto numSteps = 100;
    auto grid                      = input;
    if constexpr (fixCorners) {
      grid[0][0]         = '#';
      grid[n - 1][0]     = '#';
      grid[0][n - 1]     = '#';
      grid[n - 1][n - 1] = '#';
    }
    for (int t = 0; t < numSteps; ++t) {
      auto newGrid = grid;
      for (int j = 0; j < n; ++j) {
        for (int i = 0; i < n; ++i) {
          std::vector<char> neighbors;
          for (const auto& dd : ddirs) {
            const auto new_i = i + dd[0];
            const auto new_j = j + dd[1];
            if (0 <= new_i && new_i < n && 0 <= new_j && new_j < n) {
              neighbors.push_back(grid[new_j][new_i]);
            }
          }
          newGrid[j][i] = lightFunction(grid[j][i], neighbors);
        }
      }
      grid = newGrid;
      if constexpr (fixCorners) {
        grid[0][0]         = '#';
        grid[n - 1][0]     = '#';
        grid[0][n - 1]     = '#';
        grid[n - 1][n - 1] = '#';
      }
    }

    return std::accumulate(grid.begin(), grid.end(), 0, [](int sm, const auto& row) {
      return sm + std::count(row.begin(), row.end(), '#');
    });
  }

  std::string part_1() override { return std::to_string(simulateSteps()); }

  std::string part_2() override { return std::to_string(simulateSteps<true>()); }
};

};  // namespace AOC2015
