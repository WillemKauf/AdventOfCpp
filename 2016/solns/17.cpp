/////////////////
//// std
/////////////////
#include <cctype>
#include <limits>
#include <ranges>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2016 {

struct day_17 : public Advent_type {
  static constexpr int year = 2016;
  static constexpr int date = 17;
  const std::string input   = read_lines_string(year, date)[0];

  using Integer_type                     = int8_t;
  static constexpr Integer_type gridSize = 4;

  struct State {
    State(Integer_type x, Integer_type y, int numSteps, const std::string& pathStr)
        : x(x), y(y), numSteps(numSteps), pathStr(pathStr) {}
    Integer_type x;
    Integer_type y;
    int numSteps;
    std::string pathStr;
  };

  std::string part_1() override {
    std::queue<State> queue;
    queue.emplace(0, 0, 0, input);
    // static constexpr auto ddir = Grid::GetAllCardinalDirs(); // Cannot use, need to ensure that
    // direction map is held constant.
    std::array<std::array<Integer_type, 2>, 4> ddirs = {0, -1, 0, 1, -1, 0, 1, 0};
    const std::unordered_set<char> openDoorChars     = {'b', 'c', 'd', 'e', 'f'};
    const std::unordered_map<int, char> charMap      = {{0, 'U'}, {1, 'D'}, {2, 'L'}, {3, 'R'}};
    int minSteps                                     = std::numeric_limits<int>::max();
    std::string shortestPath;
    while (!queue.empty()) {
      const auto currState = std::move(queue.front());
      queue.pop();
      if (currState.x == gridSize - 1 && currState.y == gridSize - 1) {
        if (currState.numSteps < minSteps) {
          minSteps     = currState.numSteps;
          shortestPath = currState.pathStr.substr(input.size());
          continue;
        }
      }

      if (currState.numSteps >= minSteps) {
        continue;
      }

      const auto currHash = Hash::GetMD5Hash(currState.pathStr).substr(0, 4);
      for (auto [index, dd] : std::views::enumerate(ddirs)) {
        if (!openDoorChars.contains(std::tolower(currHash[index]))) {
          continue;
        }
        const auto newX = currState.x + dd[0];
        const auto newY = currState.y + dd[1];
        if (0 <= newX && newX < gridSize && 0 <= newY && newY < gridSize) {
          queue.emplace(newX, newY, currState.numSteps + 1, currState.pathStr + charMap.at(index));
        }
      }
    }
    return shortestPath;
  }

  std::string part_2() override {
    std::queue<State> queue;
    queue.emplace(0, 0, 0, input);
    // static constexpr auto ddir = Grid::GetAllCardinalDirs(); // Cannot use, need to ensure that
    // direction map is held constant.
    std::array<std::array<Integer_type, 2>, 4> ddirs = {0, -1, 0, 1, -1, 0, 1, 0};
    const std::unordered_set<char> openDoorChars     = {'b', 'c', 'd', 'e', 'f'};
    const std::unordered_map<int, char> charMap      = {{0, 'U'}, {1, 'D'}, {2, 'L'}, {3, 'R'}};
    int maxSteps                                     = std::numeric_limits<int>::min();
    while (!queue.empty()) {
      const auto currState = std::move(queue.front());
      queue.pop();
      if (currState.x == gridSize - 1 && currState.y == gridSize - 1) {
        maxSteps = std::max(maxSteps, currState.numSteps);
        continue;
      }

      const auto currHash = Hash::GetMD5Hash(currState.pathStr).substr(0, 4);
      for (auto [index, dd] : std::views::enumerate(ddirs)) {
        if (!openDoorChars.contains(std::tolower(currHash[index]))) {
          continue;
        }
        const auto newX = currState.x + dd[0];
        const auto newY = currState.y + dd[1];
        if (0 <= newX && newX < gridSize && 0 <= newY && newY < gridSize) {
          queue.emplace(newX, newY, currState.numSteps + 1, currState.pathStr + charMap.at(index));
        }
      }
    }
    return std::to_string(maxSteps);
  }
};

};  // namespace AOC2016
