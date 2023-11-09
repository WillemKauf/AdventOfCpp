/////////////////
//// std
/////////////////
#include <cstdint>
#include <limits>
#include <queue>
#include <variant>
#include <vector>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

namespace AOC2016 {

struct day_14 : public Advent_type {
  static constexpr int year    = 2016;
  static constexpr int date    = 14;
  const std::string input      = read_lines_string(year, date)[0];
  static constexpr int numKeys = 64;

  template <bool partTwo = false>
  int findFinalIndex() {
    static constexpr int windowSize = 1000;
    std::deque<std::pair<int, int>> hashes;
    int seenKeys   = 0;
    int finalIndex = 0;
    for (int i = 0; seenKeys < numKeys; ++i) {
      const auto toHash = input + std::to_string(i);
      auto hashStr      = Hash::GetMD5Hash(toHash);
      if constexpr (partTwo) {
        for (int i = 0; i < 2016; ++i) {
          hashStr = Hash::GetMD5Hash(hashStr);
        }
      }
      for (auto window : std::views::slide(hashStr, 5)) {
        if (window[0] == window[1] && window[1] == window[2] && window[2] == window[3] &&
            window[3] == window[4]) {
          for (auto& hash : hashes) {
            if (hash.first == window[0]) {
              if (++seenKeys == numKeys) {
                finalIndex = hash.second;
                break;
              }
              hash.first = -1;
            }
          }
        }
      }
      int toPush = -1;
      for (auto window : std::views::slide(hashStr, 3)) {
        if (window[0] == window[1] && window[1] == window[2]) {
          toPush = window[0];
          break;
        }
      }
      hashes.push_back({toPush, i});
      if (hashes.size() > windowSize) {
        hashes.pop_front();
      }
    }
    return finalIndex;
  }

  std::string part_1() override { return std::to_string(findFinalIndex()); }

  std::string part_2() override { return std::to_string(findFinalIndex<true>()); }
};

};  // namespace AOC2016
