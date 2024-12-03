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

struct day_03 : public Advent_type {
  static constexpr int year = 2024;
  static constexpr int date = 3;

  std::string part_1() override {
    const std::vector<std::vector<std::string>> input =
        read_lines_vector_regex<std::string>(year, date, "mul\\(\\d+,\\d+\\)");
    return std::to_string(
        std::accumulate(input.begin(), input.end(), int64_t{0}, [](int64_t s, const auto& v) {
          int64_t c =
              std::accumulate(v.begin(), v.end(), int64_t{0}, [](int64_t c, const auto& str) {
                const auto split_str = Strings::Split(str, ",");
                const auto a         = std::stoi(split_str[0].substr(4));
                const auto b         = std::stoi(split_str[1].substr(0, split_str[1].size()));
                return c + a * b;
              });
          return s + c;
        }));
  }

  struct Regex_type {
    static constexpr bool collect_position = true;
    using Value_type                       = std::string;
    using Position_type                    = size_t;
    Value_type v;
    Position_type p;
    auto operator<=>(const Regex_type& o) const { return p <=> o.p; }
  };

  std::string part_2() override {
    std::vector<std::vector<Regex_type>> input = read_lines_vector_regex<Regex_type>(
        year, date, "mul\\(\\d+,\\d+\\)", "don\'t\\(\\)", "do\\(\\)");

    bool do_mul = true;
    return std::to_string(
        std::accumulate(input.begin(), input.end(), int64_t{0}, [&do_mul](int64_t s, auto& v) {
          std::sort(v.begin(), v.end());
          int64_t c =
              std::accumulate(v.begin(), v.end(), int64_t{0}, [&do_mul](int64_t c, const auto& r) {
                if (r.v.substr(0, 3) == "mul" && do_mul) {
                  const auto split_str = Strings::Split(r.v, ",");
                  const auto a         = std::stoi(split_str[0].substr(4));
                  const auto b         = std::stoi(split_str[1].substr(0, split_str[1].size()));
                  return c + a * b;
                } else if (r.v.substr(0, 3) == "don") {
                  do_mul = false;
                } else if (r.v.substr(0, 2) == "do") {
                  do_mul = true;
                }
                return c;
              });
          return s + c;
        }));
  }
};

}  // namespace AOC2024
