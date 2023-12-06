/////////////////
//// std
/////////////////
#include <algorithm>
#include <deque>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"
#include "algorithm.h"

namespace AOC2018 {

struct day_09 : public Advent_type {
  static constexpr int year    = 2018;
  static constexpr int date    = 9;
  const std::vector<int> input = read_lines_vector_regex<int>(year, date, "(\\d+)")[0];

  using Int_type = uint32_t;

  Int_type Simulate(int numPlayers, int lastMarble) const {
    static constexpr auto multiple       = 23;
    static constexpr auto multipleRotate = 7;
    auto playerMap                       = std::vector<Int_type>(numPlayers, 0);
    std::deque<Int_type> marbles         = {0};

    for (Int_type currMarble = 1; currMarble <= lastMarble; ++currMarble) {
      if (currMarble % multiple == 0) {
        const auto currPlayer = currMarble % numPlayers;
        Algorithm::RotateDeque(marbles, multipleRotate);
        playerMap[currPlayer] += currMarble + marbles.front();
        marbles.pop_front();
      } else {
        Algorithm::RotateDeque(marbles, -2);
        marbles.push_front(currMarble);
      }
    }
    return *std::max_element(playerMap.begin(), playerMap.end());
  }

  std::string part_1() override { return std::to_string(Simulate(input[0], input[1])); }

  std::string part_2() override { return std::to_string(Simulate(input[0], input[1] * 100)); }
};

};  // namespace AOC2018
