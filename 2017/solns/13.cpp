/////////////////
//// std
/////////////////

/////////////////
//// local
/////////////////
#include <limits>

#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2017 {

struct day_13 : public Advent_type {
  static constexpr int year                 = 2017;
  static constexpr int date                 = 13;
  const std::vector<std::vector<int>> input = read_lines_vector_regex<int>(year, date, "(\\d+)");

  struct Scanner {
    Scanner(int depth, int range) : depth(depth), range(range) {}
    int depth;
    int range;

    // Get position of scanner if packet leaves at time t.
    int GetPosition(int t) const {
      const auto numPasses = (t + depth) / (range - 1);
      const auto currPass  = (t + depth) % (range - 1);
      // numPasses % 2 == 0 -> This is currently moving downwards, else upwards.
      return (numPasses % 2 == 0) ? currPass : range - currPass;;
    }
  };

  std::vector<Scanner> ParseInput() const {
    std::vector<Scanner> scanners;
    for (const auto& v : input) {
      scanners.emplace_back(v[0], v[1]);
    }
    return scanners;
  }

  const std::vector<Scanner> scannerVec = ParseInput();

  std::string part_1() override {
    return std::to_string(
        std::accumulate(scannerVec.begin(), scannerVec.end(), 0, [](int sm, const auto& scanner) {
          const auto pos = scanner.GetPosition(0);
          return sm + ((pos == 0) ? scanner.depth * scanner.range : 0);
        }));
  }

  std::string part_2() override {
    const auto canMakeFunc = [&](const auto& t) {
      return std::all_of(scannerVec.begin(), scannerVec.end(),
                         [t](const auto& scanner) { return scanner.GetPosition(t) != 0; });
    };

    for (int t = 0; true; ++t) {
      if (canMakeFunc(t)) {
        return std::to_string(t);
      }
    }
    std::unreachable();
  }
};

};  // namespace AOC2017
