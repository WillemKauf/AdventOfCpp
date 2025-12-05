#include <cstddef>
#include <cstdint>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"
#include "algorithm.h"
#include "data_structure.h"

namespace AOC2025 {

struct day_02 : public Advent_type {
  static constexpr int year = 2025;
  static constexpr int date = 2;
  using Vec_type            = std::vector<uint64_t>;
  const Vec_type input      = read_lines_vector_regex<uint64_t, true>(year, date, "(\\d+)");

  uint64_t invalid_id_sum_in_range(uint64_t a, uint64_t b) {
    uint64_t res = 0;
    int max_len  = Algorithm::DigitCount(b);

    for (int k = 1; k <= max_len / 2; ++k) {
      uint64_t start = static_cast<uint64_t>(std::pow(10, k - 1));
      uint64_t end   = static_cast<uint64_t>(std::pow(10, k));
      for (uint64_t s = start; s < end; ++s) {
        uint64_t n = s * static_cast<uint64_t>(std::pow(10, k)) + s;
        if (n >= a && n <= b) {
          res += n;
        }
      }
    }

    return res;
  }

  std::string part_1() override {
    uint64_t res = 0;
    for (auto pair : input | std::views::chunk(2)) {
      auto a = pair.front();
      auto b = pair.back();
      res += invalid_id_sum_in_range(a, b);
    }
    return std::to_string(res);
  }

  uint64_t invalid_id_sum_in_range_p2(uint64_t a, uint64_t b) {
    // I give up on math, use regex.
    static const boost::regex re(R"(^(\d+)\1+$)");
    uint64_t res = 0;

    for (uint64_t v = a; v <= b; ++v) {
      std::string s = std::to_string(v);

      if (boost::regex_match(s, re)) {
        res += v;
      }
    }

    return res;
  }

  std::string part_2() override {
    uint64_t res = 0;
    for (auto pair : input | std::views::chunk(2)) {
      auto a = pair.front();
      auto b = pair.back();
      res += invalid_id_sum_in_range_p2(a, b);
    }
    return std::to_string(res);
  }
};

};  // namespace AOC2025
