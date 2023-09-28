/////////////////
//// std
/////////////////
#include <unordered_set>
#include <utility>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

struct day_03 : public Advent_type {
  static constexpr int year     = 2015;
  static constexpr int date     = 3;
  const std::vector<char> input = read_single_line(year, date);

  std::string part_1() override {
    std::unordered_set<int> seen = {0};
    int x                        = 0;
    int y                        = 0;

    const auto hashFunc = [](int i, int j) {
      static const auto bigNum = int(1e8);
      return i + bigNum * j;
    };

    for (const auto& c : input) {
      switch (c) {
        case '>':
          ++x;
          break;
        case '<':
          --x;
          break;
        case '^':
          ++y;
          break;
        case 'v':
          --y;
          break;
        default:
          throw std::runtime_error("Didn't handle case in day03");
      }
      seen.insert(hashFunc(x, y));
    }
    return std::to_string(seen.size());
  }

  std::string part_2() override {
    std::unordered_set<int> seen = {0};
    std::array<int, 2> x         = {0, 0};
    std::array<int, 2> y         = {0, 0};
    int turn                     = 0;

    const auto hashFunc = [](int i, int j) {
      static const auto bigNum = int(1e8);
      return i + bigNum * j;
    };

    for (const auto& c : input) {
      switch (c) {
        case '>':
          ++x[turn];
          break;
        case '<':
          --x[turn];
          break;
        case '^':
          ++y[turn];
          break;
        case 'v':
          --y[turn];
          break;
        default:
          throw std::runtime_error("Didn't handle case in day03");
      }
      seen.insert(hashFunc(x[turn], y[turn]));
      turn ^= 1;
    }
    return std::to_string(seen.size());
  }
};
