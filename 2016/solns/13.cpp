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
#include "../common/common.h"
#include "../common/advent_base.h"
#include "../common/read_input.h"

namespace AOC2016 {

struct day_13 : public Advent_type {
  static constexpr int year = 2016;
  static constexpr int date = 13;
  const int input    = std::stoi(read_lines<std::string>(year, date)[0]);

  using Integer_type = uint16_t;

  template <typename T>
  T function(T x, T y) const {
    return x * x + 3 * x + 2 * x * y + y + y * y;
  }

  template <typename T>
  int countNumOnes(T n) const {
    return std::popcount(n);
  }

  template <typename T>
  bool isOpen(T x, T y) const {
    static std::unordered_map<uint64_t, bool> cacheMap;
    const uint64_t hashNum = static_cast<uint64_t>(function(x, y) + input);
    if (!cacheMap.contains(hashNum)) {
      const auto numBits = countNumOnes(hashNum);
      cacheMap[hashNum]  = (numBits % 2 == 0);
    }
    return cacheMap.at(hashNum);
  }

  struct State {
    Integer_type x;
    Integer_type y;
    int numSteps = 0;
  };

  std::string part_1() override {
    std::queue<State> queue;
    queue.emplace(1, 1);
    static constexpr auto finalX = 31;
    static constexpr auto finalY = 39;
    int minSteps                 = std::numeric_limits<int>::max();
    static constexpr auto ddir   = Grid::GetAllCardinalDirs();
    std::unordered_set<uint32_t> seen;
    while (!queue.empty()) {
      const auto currState = std::move(queue.front());
      queue.pop();

      const uint32_t hash = (currState.x << 16) | currState.y;
      if (seen.contains(hash)) {
        continue;
      }

      seen.insert(hash);

      if (currState.x == finalX && currState.y == finalY) {
        minSteps = std::min(minSteps, currState.numSteps);
        continue;
      }

      if (currState.numSteps >= minSteps) {
        continue;
      }

      for (const auto& dd : ddir) {
        const auto newX = currState.x + dd[0];
        const auto newY = currState.y + dd[1];
        if (newX < 0 || newY < 0) {
          continue;
        }

        if (isOpen(newX, newY)) {
          queue.emplace(newX, newY, currState.numSteps + 1);
        }
      }
    }
    return std::to_string(minSteps);
  }

  std::string part_2() override {
    std::queue<State> queue;
    queue.emplace(1, 1);
    static constexpr auto ddir = Grid::GetAllCardinalDirs();
    std::unordered_set<uint32_t> seen;
    while (!queue.empty()) {
      const auto currState = std::move(queue.front());
      queue.pop();

      const uint32_t hash = (currState.x << 16) | currState.y;
      if (seen.contains(hash)) {
        continue;
      }

      seen.insert(hash);

      if (currState.numSteps >= 50) {
        continue;
      }

      for (const auto& dd : ddir) {
        const auto newX = currState.x + dd[0];
        const auto newY = currState.y + dd[1];
        if (newX < 0 || newY < 0) {
          continue;
        }

        if (isOpen(newX, newY)) {
          queue.emplace(newX, newY, currState.numSteps + 1);
        }
      }
    }
    return std::to_string(seen.size());
  }
};

};  // namespace AOC2016
