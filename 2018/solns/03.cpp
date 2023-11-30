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

struct day_03 : public Advent_type {
  static constexpr int year                 = 2018;
  static constexpr int date                 = 3;
  const std::vector<std::vector<int>> input = read_lines_vector_regex<int>(year, date, "(\\d+)");
  static constexpr int gridSize             = 1000;

  using Row_type  = std::vector<std::unordered_set<int>>;
  using Grid_type = std::vector<Row_type>;

  Grid_type grid = Grid_type(gridSize, Row_type(gridSize));

  std::string part_1() override {
    for (const auto& v : input) {
      const auto& c  = v[0];
      const auto& i  = v[1];
      const auto& j  = v[2];
      const auto& di = v[3];
      const auto& dj = v[4];
      for (int jj = 0; jj < dj; ++jj) {
        for (int ii = 0; ii < di; ++ii) {
          grid[j + jj][i + ii].insert(c);
        }
      }
    }
    return std::to_string(std::accumulate(grid.begin(), grid.end(), 0, [](int sm, const auto& row) {
      return sm +
             std::count_if(row.begin(), row.end(), [](const auto& set) { return set.size() >= 2; });
    }));
  }

  std::string part_2() override {
    for (const auto& v : input) {
      const auto& c     = v[0];
      const auto& i     = v[1];
      const auto& j     = v[2];
      const auto& di    = v[3];
      const auto& dj    = v[4];
      bool isNonOverlap = true;
      for (int jj = 0; jj < dj; ++jj) {
        for (int ii = 0; ii < di; ++ii) {
          if (grid[j + jj][i + ii].size() > 1) {
            isNonOverlap = false;
            break;
          }
        }
        if (!isNonOverlap) {
          break;
        }
      }
      if (isNonOverlap) {
        return std::to_string(c);
      }
    }
    std::unreachable();
  }
};

};  // namespace AOC2018
