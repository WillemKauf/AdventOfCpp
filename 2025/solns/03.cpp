#include <cstddef>
#include <cstdint>
#include <numeric>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"
#include "algorithm.h"
#include "data_structure.h"

namespace AOC2025 {

struct day_03 : public Advent_type {
  static constexpr int year = 2025;
  static constexpr int date = 3;
  using Vec_type            = std::vector<std::string>;
  const Vec_type input      = read_lines<std::string>(year, date);

  std::string part_1() override {
    uint64_t res = 0;
    for (const auto& s : input) {
      int a = 0;
      int b = 0;
      for (auto [i, c] : std::views::enumerate(s)) {
        auto v = c - '0';
        if (v > a && i != s.size() - 1) {
          a = v;
          b = 0;
          continue;
        }
        if (v > b) {
          b = v;
        }
      }
      res += 10 * a + b;
    }
    return std::to_string(res);
  }

  std::string part_2() override {
    uint64_t res = 0;
    std::stack<int> st;
    static constexpr auto num_batteries = 12;
    for (const auto& s : input) {
      for (auto [i, c] : std::views::enumerate(s)) {
        auto v   = c - '0';
        auto rem = s.size() - i;
        while (!st.empty() && v > st.top() && st.size() + rem > num_batteries) {
          st.pop();
        }

        if (st.size() < num_batteries) {
          st.push(v);
        }
      }

      uint64_t exp = 1;
      while (!st.empty()) {
        res += exp * static_cast<uint64_t>(st.top());
        st.pop();
        exp *= 10;
      }
    }
    return std::to_string(res);
  }
};

};  // namespace AOC2025
