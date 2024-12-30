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
#include <unordered_set>
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

struct day_10 : public Advent_type {
  static constexpr int year = 2024;
  static constexpr int date = 10;

  using Grid_type      = std::vector<std::vector<int>>;
  using Set_type       = std::unordered_set<uint32_t>;
  const Grid_type grid = read_lines_vector<int>(year, date);

  uint64_t dfs(int i, int j, const Grid_type& grid, Set_type* seen) const {
    static constexpr auto& ddir = Grid::OrthogonalDirections;
    const auto& c               = grid[j][i];
    static const auto m         = grid.size();
    static const auto n         = grid[0].size();

    const auto hash = Grid::HashIndex<uint32_t>(i, j, n);
    if (seen && !seen->insert(hash).second) {
      return 0;
    }

    if (c == 9) {
      return 1;
    }

    uint64_t res = 0;
    for (const auto& dd : ddir) {
      const auto ii = i + dd[0];
      const auto jj = j + dd[1];
      if (Grid::InBounds(ii, jj, n, m) && grid[jj][ii] == c + 1) {
        res += dfs(ii, jj, grid, seen);
      }
    }
    return res;
  }

  template <bool part_two = false>
  uint64_t accumulate() const {
    uint64_t res = 0;
    Set_type seen;
    for (int j = 0; j < grid.size(); ++j) {
      for (int i = 0; i < grid[j].size(); ++i) {
        Set_type* seen_ptr = &seen;
        if constexpr (part_two) {
          seen_ptr = nullptr;
        } else {
          seen_ptr->clear();
        }
        const auto& c = grid[j][i];
        if (c == 0) {
          res += dfs(i, j, grid, seen_ptr);
        }
      }
    }
    return res;
  }

  std::string part_1() override { return std::to_string(accumulate()); }
  std::string part_2() override { return std::to_string(accumulate<true>()); }
};

}  // namespace AOC2024
