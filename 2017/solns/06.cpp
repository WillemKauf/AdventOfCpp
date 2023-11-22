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

struct day_06 : public Advent_type {
  static constexpr int year    = 2017;
  static constexpr int date    = 6;
  const std::vector<int> input = read_lines_vector_regex<int>(year, date, "(\\d+)")[0];

  //Part two variable.
  int numCycles = {};

  std::string part_1() override {
    auto blocks         = input;
    const int n         = blocks.size();
    const auto hashFunc = [](const auto& vec) {
      std::string hash = {};
      for (const auto& v : vec) {
        hash += std::to_string(v) + "-";
      }
      hash.pop_back();
      return hash;
    };

    int numSteps = 1;
    //Promoted from std::unordered_set<std::string> from part one.
    std::unordered_map<std::string, int> seenHashes;
    for (; true; ++numSteps) {
      auto maxIt        = std::max_element(blocks.begin(), blocks.end());
      int maxValue      = *maxIt;
      const int numEach = std::max(1, maxValue / (n - 1));
      *maxIt            = 0;

      for (auto i = (std::distance(blocks.begin(), maxIt) + 1) % n; maxValue > 0; i = (i + 1) % n) {
        blocks[i] += std::min(maxValue, numEach);
        maxValue -= numEach;
      }

      const auto hash = hashFunc(blocks);

      if (auto hashIt = seenHashes.find(hash); hashIt != seenHashes.end()) {
        numCycles = numSteps - hashIt->second;
        break;
      }

      seenHashes.insert_or_assign(hash, numSteps);
    }

    return std::to_string(numSteps);
  }

  std::string part_2() override {
    return std::to_string(numCycles);
  }
};

};  // namespace AOC2017
