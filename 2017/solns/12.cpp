/////////////////
//// std
/////////////////

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2017 {

struct day_12 : public Advent_type {
  static constexpr int year                 = 2017;
  static constexpr int date                 = 12;
  const std::vector<std::vector<int>> input = read_lines_vector_regex<int>(year, date, "(\\d+)");

  DataStructures::DSU ParseInput() const { return {static_cast<int>(input.size())}; }

  DataStructures::DSU dsu = ParseInput();

  std::string part_1() override {
    for (const auto& v : input) {
      const auto& root = v[0];
      for (const auto& child : std::views::drop(v, 1)) {
        dsu.JoinRoots(root, child);
      }
    }
    return std::to_string(dsu.GetSize(0));
  }

  std::string part_2() override { return std::to_string(dsu.GetNumSets()); }
};

};  // namespace AOC2017
