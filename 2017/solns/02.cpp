/////////////////
//// std
/////////////////
#include <algorithm>
#include <numeric>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2017 {

struct day_02 : public Advent_type {
  static constexpr int year                 = 2017;
  static constexpr int date                 = 2;
  const std::vector<std::vector<int>> input = read_lines_vector_regex<int>(year, date, "(\\d+)");

  std::string part_1() override {
    return std::to_string(
        std::accumulate(input.begin(), input.end(), 0, [](int sm, const auto& row) {
          const auto [minIt, maxIt] = std::minmax_element(row.begin(), row.end());
          return sm + (*maxIt - *minIt);
        }));
  }

  std::string part_2() override {
    return std::to_string(
        std::accumulate(input.begin(), input.end(), 0, [](int sm, const auto& row) {
          int currSm = 0;
          for (auto [i, v] : std::views::enumerate(row)) {
            for (auto vv : std::views::drop(row, i + 1)) {
              if (v % vv == 0 || vv % v == 0) {
                currSm += std::max(v, vv) / std::min(v, vv);
              }
            }
          }
          return sm + currSm;
        }));
  }
};

};  // namespace AOC2017
