/////////////////
//// std
/////////////////
#include <cstddef>
#include <cstdint>
#include <numeric>
#include <queue>
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
#include "grid.h"
#include "strings.h"

namespace AOC2024 {

struct day_05 : public Advent_type {
  static constexpr int year = 2024;
  static constexpr int date = 5;
  using Scalar_type         = uint32_t;
  using Update_type         = std::vector<Scalar_type>;
  using Set_type            = std::unordered_set<Scalar_type>;

  const std::vector<std::string> input = read_lines<std::string>(year, date);

  struct Input_type {
    std::unordered_map<Scalar_type, Set_type> ordering_rules;
    std::vector<Update_type> updates;
  };

  Input_type ParseInput() {
    bool first_part = true;
    std::unordered_map<Scalar_type, Set_type> ordering_rules;
    std::vector<Update_type> updates;
    for (const auto& line : input) {
      if (line.empty()) {
        first_part = false;
        continue;
      }
      if (first_part) {
        const auto split_str = Strings::Split(line, "|");
        const auto a         = std::stoi(split_str[0]);
        const auto b         = std::stoi(split_str[1]);
        ordering_rules[a].insert(b);
      } else {
        const auto split_str = Strings::Split(line, ",");
        Update_type update;
        update.reserve(split_str.size());
        for (const auto& v : split_str) {
          update.push_back(std::stoi(v));
        }
        updates.push_back(std::move(update));
      }
    }

    return Input_type{std::move(ordering_rules), std::move(updates)};
  }

  const Input_type input_struct = ParseInput();

  Update_type sort_update(const auto& update) const {
    auto ordering_rules = input_struct.ordering_rules;
    Set_type update_set(update.begin(), update.end());
    std::unordered_map<Scalar_type, Scalar_type> in_degrees;

    for (const auto& u : update_set) {
      for (const auto& uu : update_set) {
        if (ordering_rules[u].contains(uu)) {
          in_degrees[u];
          ++in_degrees[uu];
        }
      }
    }

    Update_type sorted_update;
    std::queue<Scalar_type> q;
    for (const auto& [n, d] : in_degrees) {
      if (d == 0) {
        q.push(n);
      }
    }
    while (!q.empty()) {
      auto v = q.front();
      q.pop();
      sorted_update.push_back(v);
      for (const auto& n : ordering_rules[v]) {
        if (--in_degrees[n] == 0 && update_set.contains(n)) {
          q.push(n);
        }
      }
    }
    return sorted_update;
  }

  Scalar_type s_part_two = 0;

  std::string part_1() override {
    const auto& updates = input_struct.updates;
    return std::to_string(std::accumulate(updates.begin(), updates.end(), Scalar_type{0},
                                          [this](Scalar_type s, const auto& update) {
                                            const auto sorted_update = sort_update(update);
                                            if (sorted_update == update) {
                                              s += update[update.size() / 2];
                                            } else {
                                              s_part_two += sorted_update[sorted_update.size() / 2];
                                            }
                                            return s;
                                          }));
  }

  std::string part_2() override { return std::to_string(s_part_two); }
};

}  // namespace AOC2024
