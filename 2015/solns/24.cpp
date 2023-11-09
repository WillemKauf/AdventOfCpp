/////////////////
//// std
/////////////////
#include <cstdint>
#include <limits>
#include <stdexcept>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

namespace AOC2015 {

struct day_24 : public Advent_type {
  static constexpr int year = 2015;
  static constexpr int date = 24;

  const std::vector<int> input = read_lines<int>(year, date);
  const int inputSize          = input.size();
  const int groupWeights       = std::accumulate(input.begin(), input.end(), 0) / 3;

  void generatePermutation(const std::vector<int>& groupSizes, int currWeight, int64_t used,
                           int index, const std::vector<int>& currGroup,
                           std::vector<std::vector<int>> currConfig,
                           std::vector<std::vector<std::vector<int>>>& perms) const {
    const bool isGroupFull = (currGroup.size() == groupSizes[index]);
    if (isGroupFull) {
      if (currWeight == groupWeights) {
        ++index;
        currWeight = 0;
        currConfig.push_back(currGroup);
        if (index == 3) {
          perms.push_back(currConfig);
          return;
        }
      } else {
        return;
      }
    }

    for (auto [weightIndex, weight] : std::views::enumerate(input)) {
      const auto shift = 1 << weightIndex;
      if (used & shift) {
        continue;
      }
      const auto newWeight = currWeight + weight;
      if (newWeight <= groupWeights) {
        auto newGroup = isGroupFull ? std::vector<int>{} : currGroup;
        newGroup.push_back(weight);
        generatePermutation(groupSizes, newWeight, used | shift, index, newGroup, currConfig,
                            perms);
      }
    }
  }

  std::vector<std::vector<std::vector<int>>> generatePermutations(int groupI, int groupJ,
                                                                  int groupK) const {
    std::vector<std::vector<std::vector<int>>> perms;
    generatePermutation({groupI, groupJ, groupK}, 0, 0, 0, {}, {}, perms);
    return perms;
  }

  int calculateQE(const std::vector<std::vector<int>>& config) const {
    return std::accumulate(config[0].begin(), config[0].end(), 1, std::multiplies<int>{});
  }

  std::string part_1() override {
    // auto minQE = std::numeric_limits<int>::max();
    // for (int i = 1; i < inputSize; ++i) {
    //   bool foundGroup = false;
    //   for (int j = 1; j < inputSize - i; ++j) {
    //     const auto k = inputSize - i - j;
    //     auto perms   = generatePermutations(i, j, k);
    //     for (const auto& perm : perms) {
    //       const auto permQE = calculateQE(perm);
    //       if (permQE < minQE) {
    //         std::cout << permQE << '\n';
    //         foundGroup = true;
    //         minQE      = permQE;
    //       }
    //     }
    //   }
    //   if (foundGroup) {
    //     break;
    //   }
    // }
    // return std::to_string(minQE);
    return "Unimplemented";
  }

  std::string part_2() override { return "Unimplemented"; }
};

};  // namespace AOC2015
