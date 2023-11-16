/////////////////
//// std
/////////////////
#include <vector>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2016 {

struct day_22 : public Advent_type {
  static constexpr int year                 = 2016;
  static constexpr int date                 = 22;
  const std::vector<std::vector<int>> input = read_lines_vector_regex<int>(
      year, date, "/dev/grid/node-x(\\d+)-y(\\d+)\\s+(\\d+)T\\s+(\\d+)T\\s+(\\d+)T");

  struct State {
    int size;
    int used;
  };

  using Map_type = std::unordered_map<int, State>;

  std::string part_1() override {
    int ans = 0;
    for (int j = 0; j < input.size(); ++j) {
      const auto& nodeA = input[j];
      if (nodeA[3] == 0) {
        continue;
      }
      for (int i = 0; i < input.size(); ++i) {
        if (i == j) {
          continue;
        }
        const auto& nodeB = input[i];
        ans += static_cast<int>(nodeA[3] <= nodeB[4]);
      }
    }
    return std::to_string(ans);
  }

  std::string part_2() override {
    const auto& n              = input.back()[0];
    static const auto getIndex = [n](const auto& i, const auto& j) { return i + j * n; };
    static constexpr auto ddir = Grid::GetAllCardinalDirs();
    return "";
  }
};

};  // namespace AOC2016
