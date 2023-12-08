/////////////////
//// std
/////////////////
#include <unordered_set>
#include <vector>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

namespace AOC2016 {

struct day_15 : public Advent_type {
  static constexpr int year                 = 2016;
  static constexpr int date                 = 15;
  const std::vector<std::vector<int>> input = read_lines_vector_regex<int>(
      year, date, "Disc #\\d+ has (\\d+) positions; at time=0, it is at position (\\d+).");

  struct Disc {
    Disc(int numPositions, int position) : numPositions(numPositions), position(position) {}
    int numPositions;
    int position;
  };

  std::vector<Disc> ParseInput() const {
    std::vector<Disc> discs;
    discs.reserve(input.size());
    for (const auto& v : input) {
      discs.emplace_back(v[0], v[1]);
    }
    return discs;
  }

  const std::vector<Disc> inputDiscs = ParseInput();

  int numSteps(const std::vector<Disc>& discs) const {
    int t              = 0;
    int maxSeenDiscs   = 0;
    int dt             = 1;
    const int numDiscs = discs.size();

    while (true) {
      std::unordered_set<int> seenDiscs;
      for (auto [index, disc] : std::views::enumerate(discs)) {
        const auto& position = (disc.position + t + (index + 1)) % disc.numPositions;
        if (position == 0) {
          seenDiscs.insert(disc.numPositions);
        }
      }

      const auto seenDiscsSize = seenDiscs.size();
      if (seenDiscsSize == numDiscs) {
        break;
      }

      if (seenDiscsSize > maxSeenDiscs) {
        maxSeenDiscs = seenDiscsSize;
        dt           = Algorithm::LCM(seenDiscs);
      }

      t += dt;
    }
    return t;
  }

  std::string part_1() override { return std::to_string(numSteps(inputDiscs)); }

  std::string part_2() override {
    auto discs = inputDiscs;
    discs.emplace_back(11, 0);
    return std::to_string(numSteps(discs));
  }
};

};  // namespace AOC2016
