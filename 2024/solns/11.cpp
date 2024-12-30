/////////////////
//// std
/////////////////
#include <cmath>
#include <complex>
#include <cstddef>
#include <cstdint>
#include <numeric>
#include <queue>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"
#include "algorithm.h"
#include "data_structure.h"
#include "grid.h"
#include "strings.h"

namespace AOC2024 {

struct day_11 : public Advent_type {
  static constexpr int year = 2024;
  static constexpr int date = 11;

  using Map_type               = std::unordered_map<uint64_t, uint64_t>;
  const std::vector<int> input = read_lines_vector_regex<int, true>(year, date, "(\\d+)");

  static std::pair<uint64_t, uint64_t> split_digits(uint64_t n) {
    auto digit_count = Algorithm::DigitCount(n);
    auto split_point = digit_count / 2;
    uint64_t divisor = static_cast<uint64_t>(std::pow(10, split_point));

    uint64_t a = n / divisor;
    uint64_t b = n % divisor;
    return std::make_pair(a, b);
  }

  static void apply_rules(uint64_t k, uint64_t c, Map_type& keys_and_counts) {
    if (c == 0) {
      return;
    }
    if (k == 0) {
      keys_and_counts[1] += c;
    } else if (Algorithm::DigitCount(k) % 2 == 0) {
      auto [l, r] = split_digits(k);
      keys_and_counts[l] += c;
      keys_and_counts[r] += c;
    } else {
      keys_and_counts[k * 2024] += c;
    }
    keys_and_counts[k] -= c;
  }

  uint64_t impl(int num_blinks) const {
    Map_type keys_and_counts;
    for (const auto& v : input) {
      ++keys_and_counts[v];
    }
    for (int i = 0; i < num_blinks; ++i) {
      auto new_keys_and_counts = keys_and_counts;
      for (const auto& [k, c] : keys_and_counts) {
        apply_rules(k, c, new_keys_and_counts);
      }
      keys_and_counts = std::move(new_keys_and_counts);
    }
    return std::accumulate(keys_and_counts.begin(), keys_and_counts.end(), uint64_t{0},
                           [](uint64_t s, const auto& p) { return s + p.second; });
  }

  std::string part_1() override {
    static constexpr auto num_blinks = 25;
    return std::to_string(impl(num_blinks));
  }

  std::string part_2() override {
    static constexpr auto num_blinks = 75;
    return std::to_string(impl(num_blinks));
  }
};

}  // namespace AOC2024
