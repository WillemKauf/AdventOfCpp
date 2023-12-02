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

namespace AOC2018 {

struct day_08 : public Advent_type {
  static constexpr int year    = 2018;
  static constexpr int date    = 8;
  const std::vector<int> input = read_lines_vector_regex<int>(year, date, "(\\d+)")[0];

  int Recurse(int& p) const {
    int sm                  = 0;
    const int numChildNodes = input[p++];
    const int numMetaData   = input[p++];

    for (int i = 0; i < numChildNodes; ++i) {
      sm += Recurse(p);
    }

    for (int i = 0; i < numMetaData; ++i) {
      sm += input[p++];
    }
    return sm;
  }

  std::string part_1() override {
    int p = 0;
    return std::to_string(Recurse(p));
  }

  int RecursePartTwo(int& p) const {
    int sm                  = 0;
    const int numChildNodes = input[p++];
    const int numMetaData   = input[p++];
    std::vector<int> sums;

    if (numChildNodes == 0) {
      for (int i = 0; i < numMetaData; ++i) {
        sm += input[p++];
      }
      return sm;
    }

    for (int i = 0; i < numChildNodes; ++i) {
      sums.push_back(RecursePartTwo(p));
    }

    for (int i = 0; i < numMetaData; ++i) {
      const auto index = input[p++] - 1;
      if (index < sums.size()) {
        sm += sums[index];
      }
    }
    return sm;
  }

  std::string part_2() override {
    int p = 0;
    return std::to_string(RecursePartTwo(p));
  }
};

};  // namespace AOC2018
