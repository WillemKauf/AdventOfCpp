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
#include "grid.h"

namespace AOC2025 {

struct day_05 : public Advent_type {
  static constexpr int year = 2025;
  static constexpr int date = 5;
  using Scalar_type         = uint64_t;
  using Vec_type            = std::vector<std::vector<Scalar_type>>;
  const Vec_type input_vec  = read_lines_vector_regex<Scalar_type>(year, date, "(\\d+)");

  struct Input {
    using Range_type  = std::pair<Scalar_type, Scalar_type>;
    using Ranges_type = std::vector<Range_type>;
    Ranges_type ranges;
    std::vector<Scalar_type> ids;
  };

  Input ParseInput() {
    Input input;
    for (const auto& v : input_vec) {
      if (v.size() == 2) {
        input.ranges.emplace_back(v[0], v[1]);
      } else {
        input.ids.emplace_back(v[0]);
      }
    }
    return input;
  }

  Input input = ParseInput();

  void SortInput(Input& input) {
    std::sort(input.ranges.begin(), input.ranges.end(),
              [](const auto& a, const auto& b) { return a.first < b.first; });
    Input::Ranges_type new_ranges;
    Scalar_type a_prev = input.ranges.front().first;
    Scalar_type b_prev = input.ranges.front().second;
    for (const auto& r : input.ranges | std::views::drop(1)) {
      Scalar_type a_next = r.first;
      Scalar_type b_next = r.second;
      if (a_next <= (b_prev + 1)) {
        b_prev = std::max(b_prev, b_next);
      } else {
        new_ranges.emplace_back(a_prev, b_prev);
        a_prev = a_next;
        b_prev = b_next;
      }
    }
    new_ranges.emplace_back(a_prev, b_prev);
    input.ranges = std::move(new_ranges);
  }

  bool ranges_contains_id(const Input::Ranges_type& r, Scalar_type id) {
    auto it =
        std::lower_bound(r.begin(), r.end(), id,
                         [](const Input::Range_type& r, Scalar_type v) { return r.second < v; });
    if (it == r.end()) {
      return false;
    }

    return it->first <= id;
  }

  std::string part_1() override {
    uint64_t res = 0;
    SortInput(input);
    for (const auto& id : input.ids) {
      res += ranges_contains_id(input.ranges, id);
    }
    return std::to_string(res);
  }

  std::string part_2() override {
    uint64_t res = 0;
    for (const auto& range : input.ranges) {
      res += range.second - range.first + 1;
    }
    return std::to_string(res);
  }
};
};  // namespace AOC2025
