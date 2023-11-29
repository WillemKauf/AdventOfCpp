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

struct day_24 : public Advent_type {
  static constexpr int year                 = 2017;
  static constexpr int date                 = 24;
  const std::vector<std::vector<int>> input = read_lines_vector_regex<int>(year, date, "(\\d+)");

  using Int_type = uint64_t;

  int dfs(int prev, Int_type used) {
    static std::unordered_map<std::string, int> dp;
    const auto hash = std::to_string(prev) + "|" + std::to_string(used);
    if (auto dpIt = dp.find(hash); dpIt != dp.end()) {
      return dpIt->second;
    }
    int max = 0;
    for (auto [i, v] : std::views::enumerate(input)) {
      const auto shift = (Int_type{1} << i);
      if (used & shift) {
        continue;
      }
      if (prev == v[0] || prev == v[1]) {
        const auto newPrev = (prev == v[0]) ? v[1] : v[0];
        const auto newUsed = used | shift;
        const auto sm      = v[0] + v[1];
        max                = std::max(max, sm + dfs(newPrev, newUsed));
      }
    }
    dp[hash] = max;
    return max;
  }

  std::string part_1() override {
    auto maxStrength = std::numeric_limits<int>::min();
    for (auto [i, v] : std::views::enumerate(input)) {
      if (v[0] == 0 || v[1] == 0) {
        const auto used = (v[0] == 0) ? v[1] : v[0];
        maxStrength     = std::max(maxStrength, used + dfs(used, (Int_type{1} << i)));
      }
    }
    return std::to_string(maxStrength);
  }

  std::string part_2() override { return ""; }
};

};  // namespace AOC2017
