/////////////////
//// std
/////////////////
#include <complex>
#include <functional>
#include <unordered_map>
#include <utility>
#include <vector>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

namespace AOC2016 {

struct day_02 : public Advent_type {
  static constexpr int year                  = 2016;
  static constexpr int date                  = 2;
  const std::vector<std::vector<char>> input = read_lines_vector(year, date);

  using Pair_type = std::pair<int, int>;
  const std::unordered_map<char, Pair_type> dirMap = {
    {'U', {0, -1}}, {'D', {0, 1}}, {'L', {-1, 0}}, {'R', {1, 0}}};

  std::string part_1() override {
    std::string keyCode;
    const auto posToNum = [](const auto& pos) { return (3 * pos.second) + pos.first + 1; };
    Pair_type pos       = {1, 1};
    for (const auto& v : input) {
      for (const auto& c : v) {
        const auto dir = dirMap.at(c);
        auto newX      = pos.first + dir.first;
        auto newY      = pos.second + dir.second;

        if (!(0 <= newX && newX < 3)) {
          newX = pos.first;
        }

        if (!(0 <= newY && newY < 3)) {
          newY = pos.second;
        }

        pos.first  = newX;
        pos.second = newY;
      }

      keyCode += std::to_string(posToNum(pos));
    }
    return keyCode;
  }

  std::string part_2() override {
    std::string keyCode;
    std::vector<std::vector<std::optional<char>>> grid = {
        {std::nullopt, std::nullopt, '1', std::nullopt, std::nullopt},
        {std::nullopt, '2',          '3', '4',          std::nullopt},
        {'5',          '6',          '7', '8',          '9'},
        {std::nullopt, 'A',          'B', 'C',          std::nullopt},
        {std::nullopt, std::nullopt, 'D', std::nullopt, std::nullopt}};
    Pair_type pos = {1, 1};
    for (const auto& v : input) {
      for (const auto& c : v) {
        const auto dir = dirMap.at(c);
        auto newX      = pos.first + dir.first;
        auto newY      = pos.second + dir.second;

        if (!(0 <= newX && newX < 5)) {
          newX = pos.first;
        }

        if (!(0 <= newY && newY < 5)) {
          newY = pos.second;
        }

        if (!grid[newY][newX].has_value()) {
          newX = pos.first;
          newY = pos.second;
        }

        pos.first  = newX;
        pos.second = newY;
      }

      keyCode += grid[pos.second][pos.first].value();
    }
    return keyCode;
  }
};

};  // namespace AOC2016
