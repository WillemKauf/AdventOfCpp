/////////////////
//// std
/////////////////
#include <algorithm>
#include <numeric>
#include <utility>
#include <vector>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

namespace AOC2015 {

struct day_02 : public Advent_type {
  static constexpr int year                 = 2015;
  static constexpr int date                 = 2;
  const std::vector<std::vector<int>> input = read_lines_vector_regex<int>(year, date, "\\d+");

  std::string part_1() override {
    return std::to_string(
        std::accumulate(input.begin(), input.end(), 0, [](int a, const auto& vec) {
          const int s1 = vec[0] * vec[1];
          const int s2 = vec[1] * vec[2];
          const int s3 = vec[2] * vec[0];
          return a + 2 * (s1 + s2 + s3) + std::min({s1, s2, s3});
        }));
  }

  std::string part_2() override {
    return std::to_string(
        std::accumulate(input.begin(), input.end(), 0, [](int a, const auto& vec) {
          const int f1 = 2 * (vec[0] + vec[1]);
          const int f2 = 2 * (vec[1] + vec[2]);
          const int f3 = 2 * (vec[2] + vec[0]);
          const int v  = std::accumulate(vec.begin(), vec.end(), 1, std::multiplies<int>());
          return a + std::min({f1, f2, f3}) + v;
        }));
  }
};

};  // namespace AOC2015
