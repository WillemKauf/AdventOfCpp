#include <cstddef>
#include <cstdint>
#include <numeric>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"
#include "algorithm.h"
#include "data_structure.h"
#include "grid.h"

namespace AOC2025 {

struct day_07 : public Advent_type {
  static constexpr int year = 2025;
  static constexpr int date = 7;
  using Vec_type            = std::vector<std::string>;
  const Vec_type grid       = read_lines<std::string>(year, date);
  const uint32_t n          = grid[0].size();

  using Hash_type = uint32_t;
  struct Input {
    std::unordered_set<Hash_type> splitters;
    std::unordered_set<Hash_type> beams;
  };

  Input ParseInput() {
    Input input;
    auto start_x = n / 2;
    input.beams.insert(Grid::HashIndex<Hash_type>(start_x, 0, n));
    for (int j = 0; j < grid.size(); ++j) {
      for (int i = 0; i < grid[j].size(); ++i) {
        if (grid[j][i] == '^') {
          auto hash = Grid::HashIndex<Hash_type>(i, j, n);
          input.splitters.insert(hash);
        }
      }
    }
    return input;
  }

  const Input initial_conditions = ParseInput();

  std::string part_1() override {
    uint64_t res     = 0;
    auto state       = initial_conditions;
    const auto y_max = grid.size();
    while (!state.beams.empty()) {
      std::unordered_set<Hash_type> new_beams;
      for (const auto& beam : state.beams) {
        auto [x, y] = Grid::HashToPosition<uint32_t>(beam, n);
        ++y;
        if (y < y_max) {
          auto new_pos_hash = Grid::HashIndex<Hash_type>(x, y, n);
          if (state.splitters.contains(new_pos_hash)) {
            auto new_pos_hash_l = Grid::HashIndex<Hash_type>(x - 1, y, n);
            auto new_pos_hash_r = Grid::HashIndex<Hash_type>(x + 1, y, n);
            new_beams.insert(new_pos_hash_l);
            new_beams.insert(new_pos_hash_r);
            ++res;
          } else {
            new_beams.insert(new_pos_hash);
          }
        }
      }
      state.beams = std::move(new_beams);
    }
    return std::to_string(res);
  }

  std::unordered_map<Hash_type, uint64_t> memo;

  uint64_t simulate(Input state) {
    if (state.beams.empty()) {
      return 1;
    }
    uint64_t res            = 0;
    static const auto y_max = grid.size();
    auto beam               = *state.beams.begin();
    auto it                 = memo.find(beam);
    if (it != memo.end()) {
      return it->second;
    }
    auto [x, y] = Grid::HashToPosition<uint32_t>(beam, n);
    ++y;
    auto new_pos_hash = Grid::HashIndex<Hash_type>(x, y, n);
    if (initial_conditions.splitters.contains(new_pos_hash)) {
      {
        auto new_pos_hash_l = Grid::HashIndex<Hash_type>(x - 1, y, n);
        auto state_l        = Input{.beams = std::unordered_set<Hash_type>{new_pos_hash_l}};
        res += simulate(std::move(state_l));
      }
      {
        auto new_pos_hash_r = Grid::HashIndex<Hash_type>(x + 1, y, n);
        auto state_r        = Input{.beams = std::unordered_set<Hash_type>{new_pos_hash_r}};
        res += simulate(std::move(state_r));
      }
    } else {
      auto new_state = Input{};
      if (y < y_max) {
        new_state.beams.insert(new_pos_hash);
      }
      res += simulate(std::move(new_state));
    }

    memo[beam] = res;
    return res;
  }

  std::string part_2() override { return std::to_string(simulate(initial_conditions)); }
};

};  // namespace AOC2025
