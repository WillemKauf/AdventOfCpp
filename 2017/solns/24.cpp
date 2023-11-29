/////////////////
//// std
/////////////////
#include <unordered_map>

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

  int dfsPartOne(int prev, Int_type used, int sm) {
    static std::unordered_map<std::string, int> dp;
    const auto hash = std::to_string(prev) + "|" + std::to_string(used);
    if (auto dpIt = dp.find(hash); dpIt != dp.end()) {
      return dpIt->second;
    }
    int max = sm;
    for (auto [i, v] : std::views::enumerate(input)) {
      const auto shift = (Int_type{1} << i);
      if (used & shift) {
        continue;
      }
      if (prev == v[0] || prev == v[1]) {
        const auto newPrev = (prev == v[0]) ? v[1] : v[0];
        const auto newUsed = used | shift;
        const auto newSm   = sm + v[0] + v[1];
        max                = std::max(max, dfsPartOne(newPrev, newUsed, newSm));
      }
    }
    dp[hash] = max;
    return max;
  }

  std::pair<int, int> dfsPartTwo(int prev, Int_type used, int sm) {
    static std::unordered_map<std::string, std::pair<int, int>> dp;
    const auto hash = std::to_string(prev) + "|" + std::to_string(used);

    if (auto dpIt = dp.find(hash); dpIt != dp.end()) {
      return dpIt->second;
    }

    int max     = sm;
    int maxSize = std::popcount(used);
    for (auto [i, v] : std::views::enumerate(input)) {
      const auto shift = (Int_type{1} << i);
      if (used & shift) {
        continue;
      }
      if (prev == v[0] || prev == v[1]) {
        const auto newPrev = (prev == v[0]) ? v[1] : v[0];
        const auto newUsed = used | shift;
        const auto newSm   = sm + v[0] + v[1];
        const auto dfsPair = dfsPartTwo(newPrev, newUsed, newSm);
        if (dfsPair.second > maxSize) {
          max     = dfsPair.first;
          maxSize = dfsPair.second;
        } else if (dfsPair.second == maxSize) {
          max = std::max(max, dfsPair.first);
        }
      }
    }
    dp[hash] = {max, maxSize};
    return {max, maxSize};
  }

  std::string part_1() override { return std::to_string(dfsPartOne(0, 0, 0)); }

  std::string part_2() override { return std::to_string(dfsPartTwo(0, 0, 0).first); }
};

};  // namespace AOC2017
