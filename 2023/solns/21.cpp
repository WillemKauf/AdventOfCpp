/////////////////
//// std
/////////////////

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2023 {

struct day_21 : public Advent_type {
  static constexpr int year            = 2023;
  static constexpr int date            = 21;
  const std::vector<std::string> input = read_lines<std::string>(year, date);

  static constexpr auto ddir = Grid::OrthogonalDirections;

  using PosInt_type = int32_t;
  using Pos_type    = std::pair<PosInt_type, PosInt_type>;

  static constexpr auto sizeOfInt  = sizeof(PosInt_type) * 8;
  static constexpr auto bitSetSize = 2 * (sizeOfInt + 1);
  using Hash_type                  = std::bitset<bitSetSize>;

  static Hash_type Hash(PosInt_type i, PosInt_type j) {
    Hash_type hash(static_cast<uint64_t>(std::abs(i)) |
                   (static_cast<uint64_t>(std::abs(j)) << sizeOfInt));
    hash[bitSetSize - 1] = (i < 0);
    hash[bitSetSize - 2] = (j < 0);
    return hash;
  }

  struct State {
    Pos_type pos;
    int numSteps;
  };

  template <bool partTwo = false>
  uint64_t PathFind() const {
    const int m             = input.size();
    const int n             = input[0].size();
    const Pos_type startPos = [&]() -> Pos_type {
      for (const auto& [j, row] : std::views::enumerate(input)) {
        for (const auto& [i, c] : std::views::enumerate(row)) {
          if (c == 'S') {
            return {i, j};
          }
        }
      }
      throw std::runtime_error("No starting point");
    }();

    int numReachable = 0;

    static constexpr int maxSteps = (partTwo) ? 500 : 64;
    const auto seenParity         = (maxSteps % 2 != 0);

    std::unordered_set<Hash_type> seen;
    std::queue<State> q;
    q.emplace(startPos, 0);

    while (!q.empty()) {
      const auto curr = std::move(q.front());
      q.pop();
      const auto& [i, j]   = curr.pos;
      const auto& numSteps = curr.numSteps;
      const auto hash      = Hash(i, j);

      if (seen.contains(hash)) {
        continue;
      }

      seen.insert(hash);

      if (seenParity == (numSteps % 2)) {
        ++numReachable;
      }

      if (numSteps == maxSteps) {
        continue;
      }

      for (const auto& dd : ddir) {
        const auto ii = i + dd[0];
        const auto jj = j + dd[1];
        if constexpr (partTwo) {
          const auto realII = Algorithm::Modulo(ii, n);
          const auto realJJ = Algorithm::Modulo(jj, m);
          if (input[realJJ][realII] == '.' || input[realJJ][realII] == 'S') {
            q.emplace(Pos_type{ii, jj}, numSteps + 1);
          }
        } else {
          if (Grid::InBounds(ii, jj, n, m)) {
            if (input[jj][ii] == '.') {
              q.emplace(Pos_type{ii, jj}, numSteps + 1);
            }
          }
        }
      }
    }
    return numReachable;
  }

  std::string part_1() override { return std::to_string(PathFind()); }

  std::string part_2() override { return std::to_string(PathFind<true>()); }
};

};  // namespace AOC2023
