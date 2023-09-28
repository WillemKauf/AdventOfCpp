/////////////////
//// std
/////////////////
#include <limits>
#include <stdexcept>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

struct day_17 : public Advent_type {
  static constexpr int year = 2015;
  static constexpr int date = 17;

  const std::vector<int> input     = read_lines<int>(year, date);
  static constexpr int totalLiters = 150;

  std::string part_1() override {
    std::vector<int> dp(totalLiters + 1, 0);
    dp[0] = 1;
    for (const auto& container : input) {
      for (int i = totalLiters; i > container - 1; --i) {
        const auto index = i - container;
        dp[i] += dp[index];
      }
    }
    return std::to_string(dp[totalLiters]);
  }

  std::string part_2() override {
    std::vector<int> dp(totalLiters + 1, totalLiters + 1);
    std::vector<int> combs(totalLiters + 1, 0);
    dp[0]    = 0;
    combs[0] = 1;
    int ans  = 0;
    for (const auto& container : input) {
      for (int i = totalLiters; i > container - 1; --i) {
        const auto index     = i - container;
        const auto isGreater = dp[i] > dp[index] + 1;
        if (isGreater) {
          dp[i]    = dp[index] + 1;
          combs[i] = combs[index];
        } else if (dp[index] + 1 == dp[i]) {
          combs[i] += combs[index];
        }
      }
    }
    return std::to_string(combs[totalLiters]);
  }
};
