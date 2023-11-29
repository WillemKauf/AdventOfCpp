/////////////////
//// std
/////////////////

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2018 {

struct day_01 : public Advent_type {
  static constexpr int year    = 2018;
  static constexpr int date    = 1;
  const std::vector<int> input = read_lines<int>(year, date);

  std::string part_1() override { return std::to_string(Algorithm::Sum(input)); }

  std::string part_2() override {
    std::unordered_set<int> seenFreqs;
    int freq = 0;
    while(true){
      for (const auto& change : input) {
        if (seenFreqs.contains(freq)) {
          return std::to_string(freq);
        }
        seenFreqs.insert(freq);
        freq += change;
      }
    }
    std::unreachable();
  }
};

};  // namespace AOC2018
