/////////////////
//// std
/////////////////
#include <cmath>
#include <complex>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <numeric>
#include <queue>
#include <stack>
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
#include "algorithm.h"
#include "data_structure.h"
#include "grid.h"
#include "strings.h"

namespace AOC2024 {

struct day_18 : public Advent_type {
  static constexpr int year = 2024;
  static constexpr int date = 18;

  // static constexpr int mem_space = 6 + 1;
  static constexpr int mem_space = 70 + 1;

  const std::vector<std::vector<uint32_t>> input =
      read_lines_vector_regex<uint32_t>(year, date, "(\\d+)");

  using Grid_type = std::vector<std::vector<char>>;
  Grid_type make_grid(int take = std::numeric_limits<int>::max()) {
    Grid_type grid(mem_space, std::vector<char>(mem_space, '.'));
    for (const auto& v : input | std::views::take(take)) {
      grid[v[1]][v[0]] = '#';
    }
    return grid;
  }

  struct State {
    uint32_t x{0};
    uint32_t y{0};
    uint32_t num_steps{0};
    auto operator<=>(const State& o) const {
      auto d  = (mem_space - x) * (mem_space - x) + (mem_space - y) * (mem_space - y);
      auto od = (mem_space - o.x) * (mem_space - o.x) + (mem_space - o.y) * (mem_space - o.y);
      return std::tie(d, num_steps) <=> std::tie(od, o.num_steps);
    }
  };

  std::string part_1() override {
    auto grid = make_grid(1024);
    std::priority_queue<State> pq;
    pq.emplace();
    const auto ddir    = Grid::GetAllCardinalDirs();
    uint32_t min_steps = std::numeric_limits<uint32_t>::max();
    using Hash_type    = uint64_t;
    using Cost_type    = uint32_t;
    std::unordered_map<Hash_type, Cost_type> costs;
    while (!pq.empty()) {
      auto state = std::move(pq.top());
      pq.pop();
      auto hash      = Grid::HashIndex<Hash_type>(state.x, state.y, mem_space);
      auto it        = costs.find(hash);
      Cost_type cost = std::numeric_limits<Cost_type>::max();
      if (it != costs.end()) {
        cost = it->second;
      }
      if (state.x == mem_space - 1 && state.y == mem_space - 1) {
        min_steps = std::min(state.num_steps, min_steps);
      }
      for (const auto& dd : ddir) {
        auto xx = state.x + dd[0];
        auto yy = state.y + dd[1];
        if (Grid::InBounds(xx, yy, mem_space, mem_space)) {
          auto next_steps = state.num_steps + 1;
          if (grid[yy][xx] == '.' && next_steps < cost) {
            pq.push(State{.x = xx, .y = yy, .num_steps = next_steps});
            costs[hash] = next_steps;
          }
        }
      }
    }
    return std::to_string(min_steps);
  }

  std::string part_2() override {
    DataStructure::DSU dsu(mem_space * mem_space);
    const auto ddir = Grid::GetAllCardinalDirs();
    using Hash_type = uint16_t;
    auto grid       = make_grid();
    auto start_hash = Grid::HashIndex<Hash_type>(0, 0, mem_space);
    auto end_hash   = Grid::HashIndex<Hash_type>(mem_space - 1, mem_space - 1, mem_space);
    for (int j = 0; j < grid.size(); ++j) {
      for (int i = 0; i < grid[j].size(); ++i) {
        if (grid[j][i] == '#') {
          continue;
        }
        auto hash = Grid::HashIndex<Hash_type>(i, j, mem_space);
        for (const auto& dd : ddir) {
          auto ii = i + dd[0];
          auto jj = j + dd[1];
          if (Grid::InBounds(ii, jj, mem_space, mem_space) && grid[jj][ii] == '.') {
            auto dhash = Grid::HashIndex<Hash_type>(ii, jj, mem_space);
            dsu.JoinRoots(hash, dhash);
          }
        }
      }
    }

    for (const auto& xy : input | std::views::reverse) {
      const auto& x = xy[0];
      const auto& y = xy[1];
      auto hash     = Grid::HashIndex<Hash_type>(x, y, mem_space);
      for (const auto& dd : ddir) {
        auto xx = x + dd[0];
        auto yy = y + dd[1];
        if (Grid::InBounds(xx, yy, mem_space, mem_space) && grid[yy][xx] == '.') {
          auto dhash = Grid::HashIndex<Hash_type>(xx, yy, mem_space);
          dsu.JoinRoots(hash, dhash);
        }
      }
      if (dsu.GetRoot(start_hash) == dsu.GetRoot(end_hash)) {
        return std::to_string(x) + "," + std::to_string(y);
      }

      grid[y][x] = '.';
    }

    std::unreachable();
  }
};

}  // namespace AOC2024
