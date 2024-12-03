/////////////////
//// std
/////////////////
#include <cstddef>
#include <cstdint>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"
#include "data_structure.h"

namespace AOC2024 {

struct day_02 : public Advent_type {
  static constexpr int year         = 2024;
  static constexpr int date         = 2;
  using Scalar_type                 = int64_t;
  using Vec_type                    = std::vector<Scalar_type>;
  const std::vector<Vec_type> input = read_lines_vector_regex<Scalar_type>(year, date, "(\\d+)");

  template <typename Func_type>
  Scalar_type accumulate_safe(Func_type is_safe_func) const {
    return std::accumulate(input.begin(), input.end(), Scalar_type{0},
                           [is_safe_func](Scalar_type s, const Vec_type& v) {
                             auto reverse_v = v;
                             std::reverse(reverse_v.begin(), reverse_v.end());
                             return s + (is_safe_func(v) || is_safe_func(reverse_v));
                           });
  }

  template <bool retry>
  static bool is_safe_func(const Vec_type& v) {
    for (const auto& [ind, p] : std::views::enumerate(std::views::slide(v, 2))) {
      const auto d = p[1] - p[0];
      if (d < 1 || d > 3) {
        if constexpr (retry) {
          Vec_type vec_a = v;
          vec_a.erase(vec_a.begin() + ind);
          Vec_type vec_b = v;
          vec_b.erase(vec_b.begin() + ind + 1);

          return is_safe_func<false>(vec_a) || is_safe_func<false>(vec_b);
        } else {
          return false;
        }
      }
    }
    return true;
  }

  std::string part_1() override { return std::to_string(accumulate_safe(is_safe_func<false>)); }

  std::string part_2() override { return std::to_string(accumulate_safe(is_safe_func<true>)); }
};

}  // namespace AOC2024
