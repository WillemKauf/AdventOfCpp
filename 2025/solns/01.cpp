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

struct day_01 : public Advent_type {
  static constexpr int year = 2025;
  static constexpr int date = 1;
  using Vec_type            = std::vector<std::string>;
  const std::vector<Vec_type> input =
      read_lines_vector_regex<std::string>(year, date, "(\\w)(\\d+)");
  static constexpr int dial_length = 99;
  static constexpr int dial_mod    = dial_length + 1;
  static constexpr int dial_start  = 50;

  std::string part_1() override {
    int dial = dial_start;
    return std::to_string(
        std::accumulate(input.begin(), input.end(), 0, [&](int sum, const auto& v) -> int {
          auto dir  = v[0];
          auto mag  = std::stoi(v[1]);
          auto card = dir == "L" ? -1 : 1;
          dial      = Algorithm::Modulo(dial + card * mag, dial_mod);
          return sum + (dial == 0);
        }));
  }

  std::string part_2() override {
    int dial = dial_start;
    return std::to_string(
        std::accumulate(input.begin(), input.end(), 0, [&](int sum, const auto& v) -> int {
          auto dir              = v[0];
          auto mag              = std::stoi(v[1]);
          auto card             = dir == "L" ? -1 : 1;
          auto dial_shifted     = dir == "L" ? Algorithm::Modulo(dial_mod - dial, dial_mod) : dial;
          auto new_dial_shifted = dial_shifted + mag;
          auto num_rots         = new_dial_shifted / dial_mod + (new_dial_shifted == 0);
          dial                  = Algorithm::Modulo(dial + card * mag, dial_mod);
          return sum + num_rots;
        }));
  }
};

};  // namespace AOC2025
