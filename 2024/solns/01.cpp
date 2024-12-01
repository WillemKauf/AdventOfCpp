/////////////////
//// std
/////////////////
#include <bits/ranges_algo.h>

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

struct day_01 : public Advent_type {
  static constexpr int year = 2024;
  static constexpr int date = 1;
  using Scalar_type         = int64_t;
  using Vec_type            = std::vector<Scalar_type>;
  const std::vector<Vec_type> input =
      read_lines_vector_regex<Scalar_type>(year, date, "(\\d+)   (\\d+)");

  Vec_type make_list(size_t index, bool sorted = false) {
    auto list_view =
        input | std::ranges::views::transform([index](const auto& v) { return v[index]; });
    Vec_type v = Vec_type(list_view.begin(), list_view.end());
    if (sorted) {
      std::ranges::sort(v);
    }
    return v;
  }

  std::string part_1() override {
    const auto list_one = make_list(0, true);
    const auto list_two = make_list(1, true);
    const auto zip_view = std::views::zip(list_one, list_two);
    return std::to_string(
        std::accumulate(zip_view.begin(), zip_view.end(), Scalar_type{0},
                        [](Scalar_type s, const std::tuple<Scalar_type, Scalar_type>& z) {
                          return s + std::abs((std::get<1>(z) - std::get<0>(z)));
                        }));
  }

  std::string part_2() override {
    const auto list_one = make_list(0);
    const auto list_two = make_list(1);
    auto freq_map       = DataStructure::Counter<Vec_type>(list_two);
    return std::to_string(
        std::accumulate(list_one.begin(), list_one.end(), Scalar_type{0},
                        [&freq_map](Scalar_type s, Scalar_type a) { return s + a * freq_map[a]; }));
  }
};

};  // namespace AOC2024
