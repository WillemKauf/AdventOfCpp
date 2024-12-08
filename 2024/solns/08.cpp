/////////////////
//// std
/////////////////
#include <cmath>
#include <complex>
#include <cstddef>
#include <cstdint>
#include <numeric>
#include <queue>
#include <string>
#include <type_traits>
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

struct day_08 : public Advent_type {
  static constexpr int year = 2024;
  static constexpr int date = 8;

  using Pos_type       = std::pair<size_t, size_t>;
  using Map_type       = std::unordered_map<char, std::vector<Pos_type>>;
  using Grid_type      = std::vector<std::vector<char>>;
  const Grid_type grid = read_lines_vector(year, date);
  const size_t m       = grid.size();
  const size_t n       = grid[0].size();

  Map_type ParseInput() const {
    Map_type mp;
    for (size_t j = 0; j < m; ++j) {
      for (size_t i = 0; i < n; ++i) {
        const auto c = grid[j][i];
        if (c != '.') {
          mp[c].emplace_back(i, j);
        }
      }
    }
    return mp;
  }

  const Map_type mp = ParseInput();

  template <bool part_two = false>
  uint32_t count_antinodes() {
    std::unordered_set<int> seen;
    for (const auto& [c, vec] : mp) {
      for (size_t i = 0; i < vec.size(); ++i) {
        const auto p1 = vec[i];
        if constexpr (part_two) {
          seen.insert(Grid::HashIndex<int>(p1.first, p1.second, n));
        }
        for (size_t j = i + 1; j < vec.size(); ++j) {
          const auto p2 = vec[j];
          const int dx  = p2.first - p1.first;
          const int dy  = p2.second - p1.second;
          const auto ax = std::abs(dx);
          const auto ay = std::abs(dy);
          const auto sx = Algorithm::Sign(dx);
          const auto sy = Algorithm::Sign(dy);
          int dx_1;
          int dy_1;
          int dx_2;
          int dy_2;

          if (sx == -1) {
            dx_1 = ax;
            dx_2 = -ax;
          } else {
            dx_1 = -ax;
            dx_2 = ax;
          }
          if (sy == -1) {
            dy_1 = ay;
            dy_2 = -ay;
          } else {
            dy_1 = -ay;
            dy_2 = ay;
          }

          auto nx_1 = p1.first + dx_1;
          auto nx_2 = p2.first + dx_2;
          auto ny_1 = p1.second + dy_1;
          auto ny_2 = p2.second + dy_2;

          while (Grid::InBounds(nx_1, ny_1, m, n)) {
            seen.insert(Grid::HashIndex<int>(nx_1, ny_1, n));
            nx_1 += dx_1;
            ny_1 += dy_1;
            if constexpr (!part_two) {
              break;
            }
          }
          while (Grid::InBounds(nx_2, ny_2, m, n)) {
            seen.insert(Grid::HashIndex<int>(nx_2, ny_2, n));
            nx_2 += dx_2;
            ny_2 += dy_2;
            if constexpr (!part_two) {
              break;
            }
          }
        }
      }
    }
    return seen.size();
  }

  std::string part_1() override { return std::to_string(count_antinodes()); }

  std::string part_2() override { return std::to_string(count_antinodes<true>()); }
};

}  // namespace AOC2024
