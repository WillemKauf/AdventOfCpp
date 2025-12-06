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

struct day_16 : public Advent_type {
  static constexpr int year = 2024;
  static constexpr int date = 16;

  using Pos_type   = int16_t;
  using Grid_type  = std::vector<std::vector<char>>;
  using Cost_type  = int64_t;
  using Hash_type  = int64_t;
  using Costs_type = std::unordered_map<Hash_type, Cost_type>;

  const Grid_type input = read_lines_vector(year, date);

  struct State {
    Pos_type x;
    Pos_type y;
    std::complex<Pos_type> d;
    Cost_type steps = 0;
    std::vector<std::pair<Pos_type, Pos_type>> history;
    auto operator<=>(const State& o) const { return o.steps <=> steps; }
  };

  Pos_type ex;
  Pos_type ey;

  uint32_t p2_ans;

  State get_starting_state() {
    State s{.x = 0, .y = 0, .d = std::complex<Pos_type>(1, 0)};
    for (int j = 0; j < input.size(); ++j) {
      for (int i = 0; i < input[j].size(); ++i) {
        if (input[j][i] == 'S') {
          s.x = i;
          s.y = j;
        }
        if (input[j][i] == 'E') {
          ex = i;
          ey = j;
        }
      }
    }
    return s;
  }

  const State starting_state = get_starting_state();

  std::vector<State> compute_next_states(const State& s, Cost_type curr_min, Costs_type& costs) {
    std::vector<State> next_states;
    auto ddir = Grid::GetAllCardinalDirs();
    for (const auto& dd : ddir) {
      Pos_type x = s.x + dd[0];
      Pos_type y = s.y + dd[1];
      if (Grid::InBounds(x, y, input[0].size(), input.size()) && input[y][x] != '#') {
        auto c       = std::complex<Pos_type>(dd[0], -dd[1]);
        Pos_type dot = s.d.real() * c.real() + s.d.imag() * c.imag();
        Cost_type next_steps{0};
        if (dot == 0) {
          next_steps = s.steps + 1001;
          // 90 degree rot
        } else if (dot == 1) {
          // No rot
          next_steps = s.steps + 1;
        } else if (dot == -1) {
          // 180 degree rot
          next_steps = s.steps + 2001;
        }
        auto hsh = Grid::HashIndexAndDirection<Hash_type>(x, y, c.real(), c.imag(), input[0].size(),
                                                          input.size());
        Cost_type cost = std::numeric_limits<Cost_type>::max();
        auto it        = costs.find(hsh);
        if (it != costs.end()) {
          cost = it->second;
        }
        if (next_steps <= curr_min && next_steps <= cost) {
          auto next_state =
              State{.x = x, .y = y, .d = c, .steps = next_steps, .history = s.history};
          next_state.history.push_back(std::make_pair(s.x, s.y));
          next_states.push_back(next_state);

          costs[hsh] = next_steps;
        }
      }
    }
    return next_states;
  }

  std::string part_1() override {
    Cost_type res = std::numeric_limits<Cost_type>::max();
    Costs_type cost;
    std::priority_queue<State> q;
    q.push(starting_state);
    std::unordered_set<Hash_type> set;
    while (!q.empty()) {
      auto s = std::move(q.top());
      q.pop();

      if (s.x == ex && s.y == ey) {
        if (s.steps <= res) {
          for (auto& p : s.history) {
            set.insert(Grid::HashIndex<Hash_type>(p.first, p.second, input[0].size()));
          }
          set.insert(Grid::HashIndex<Hash_type>(s.x, s.y, input[0].size()));
          res = std::min(res, s.steps);
        }
        continue;
      }

      if (s.steps >= res) {
        continue;
      }

      for (auto& next_state : compute_next_states(s, res, cost)) {
        q.push(std::move(next_state));
      }
    }

    p2_ans = set.size();
    return std::to_string(res);
  }

  std::string part_2() override { return std::to_string(p2_ans); }
};

}  // namespace AOC2024
