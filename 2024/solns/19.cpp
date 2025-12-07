/////////////////
//// std
/////////////////
#include <cmath>
#include <complex>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <numeric>
#include <queue>
#include <stack>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
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

struct day_19 : public Advent_type {
  static constexpr int year = 2024;
  static constexpr int date = 19;

  const std::vector<std::vector<std::string>> input_vec =
      read_lines_vector_regex<std::string>(year, date, "(\\w+)");

  struct Input {
    std::vector<std::string> avail;
    std::vector<std::string> desired;
  };

  Input ParseInput() {
    Input input;
    input.avail = input_vec[0];
    for (const auto& v : input_vec | std::views::drop(1)) {
      for (const auto& vv : v) {
        input.desired.push_back(vv);
      }
    }

    return input;
  }

  const Input input = ParseInput();

  std::unordered_set<std::string> possible;

  bool is_possible(const std::string& desired, const std::vector<std::string>& avail) {
    if (desired.empty()) {
      return true;
    }
    if (possible.contains(desired)) {
      return true;
    }
    bool p = false;
    for (const auto& a : avail) {
      if (desired.size() < a.size()) {
        continue;
      }
      if (desired.substr(0, a.size()) == a) {
        p |= is_possible(desired.substr(a.size()), avail);
        if (p) {
          break;
        }
      }
    }

    if (p) {
      possible.insert(desired);
    }
    return p;
  }

  std::string part_1() override {
    uint32_t res = 0;
    for (const auto& desired : input.desired) {
      res += is_possible(desired, input.avail);
    }

    return std::to_string(res);
  }

  std::unordered_map<std::string, uint64_t> possible_map;

  uint64_t count_possible(const std::string& desired, const std::vector<std::string>& avail) {
    if (desired.empty()) {
      return 1;
    }
    auto it = possible_map.find(desired);
    if (it != possible_map.end()) {
      return it->second;
    }
    uint64_t p{0};
    for (const auto& a : avail) {
      if (desired.size() < a.size()) {
        continue;
      }
      if (desired.substr(0, a.size()) == a) {
        p += count_possible(desired.substr(a.size()), avail);
      }
    }

    possible_map[desired] += p;

    return p;
  }

  std::string part_2() override {
    uint64_t res = 0;
    for (const auto& desired : input.desired) {
      res += count_possible(desired, input.avail);
    }

    return std::to_string(res);
  }
};

}  // namespace AOC2024
