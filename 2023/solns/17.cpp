/////////////////
//// std
/////////////////
#include <cstdint>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2023 {

struct day_17 : public Advent_type {
  static constexpr int year                 = 2023;
  static constexpr int date                 = 17;
  const std::vector<std::vector<int>> input = read_lines_vector_regex<int>(year, date, "(\\d)");

  static constexpr auto ddir = Grid::OrthogonalDirections;

  using Hash_type    = uint64_t;
  using PosInt_type  = uint8_t;
  using DirInt_type  = int8_t;
  using HeatInt_type = uint16_t;
  using Pos_type     = std::pair<PosInt_type, PosInt_type>;
  using Dir_type     = std::pair<DirInt_type, DirInt_type>;

  struct State {
    State(const Pos_type& pos, const Dir_type& dir, PosInt_type numSteps = 0,
          PosInt_type numStepsDir = 0, HeatInt_type heatLoss = 0)
        : pos(pos), dir(dir), numSteps(numSteps), numStepsDir(numStepsDir), heatLoss(heatLoss) {}
    Pos_type pos;
    Dir_type dir;
    PosInt_type numSteps;
    PosInt_type numStepsDir;
    HeatInt_type heatLoss;

    Hash_type Hash() const {
      static constexpr auto posIntSize = sizeof(PosInt_type) * 8;
      Hash_type hash                   = {};
      const PosInt_type di             = (dir.first == 0) ? 0 : ((dir.first > 0) ? 1 : 2);
      const PosInt_type dj             = (dir.second == 0) ? 0 : ((dir.second > 0) ? 1 : 2);
      hash |= (pos.first);
      hash |= (static_cast<Hash_type>(pos.second) << posIntSize);
      hash |= (static_cast<Hash_type>(di) << (2 * posIntSize));
      hash |= (static_cast<Hash_type>(dj) << (3 * posIntSize));
      hash |= (static_cast<Hash_type>(numStepsDir) << (4 * posIntSize));
      return hash;
    }
  };

  std::string part_1() override {
    static constexpr int maxStepsDir = 3;
    const auto m                     = input.size();
    const auto n                     = input[0].size();

    HeatInt_type minLoss = std::numeric_limits<HeatInt_type>::max();

    std::queue<State> q;
    q.emplace(Pos_type{0, 0}, Dir_type{1, 0});  // Right
    q.emplace(Pos_type{0, 0}, Dir_type{0, 1});  // Down

    std::unordered_map<Hash_type, HeatInt_type> seenHashes;

    while (!q.empty()) {
      const auto curr = std::move(q.front());
      q.pop();
      const auto& [i, j]      = curr.pos;
      const auto& [di, dj]    = curr.dir;
      const auto& numSteps    = curr.numSteps;
      const auto& numStepsDir = curr.numStepsDir;
      const auto& heatLoss    = curr.heatLoss;
      if (heatLoss >= minLoss) {
        continue;
      }
      if (i == n - 1 && j == m - 1) {
        minLoss = std::min(minLoss, heatLoss);
        continue;
      }
      const auto hash = curr.Hash();
      if (auto seenHashIt = seenHashes.find(hash); seenHashIt != seenHashes.end()) {
        if (seenHashIt->second <= heatLoss) {
          continue;
        }
      }
      seenHashes[hash] = heatLoss;
      for (const auto& dd : ddir) {
        const auto ii = i + dd[0];
        const auto jj = j + dd[1];
        if (Grid::InBounds(ii, jj, n, m)) {
          if (dd[0] == di && dd[1] == dj) {
            if (numStepsDir < maxStepsDir) {
              q.emplace(Pos_type{ii, jj}, Dir_type{di, dj}, numSteps + 1, numStepsDir + 1,
                        heatLoss + input[jj][ii]);
            }
          } else if (dd[0] == -di || dd[1] == -dj) {
            continue;
          } else {
            q.emplace(Pos_type{ii, jj}, Dir_type{dd[0], dd[1]}, numSteps + 1, 1,
                      heatLoss + input[jj][ii]);
          }
        }
      }
    }

    return std::to_string(minLoss);
  }

  std::string part_2() override {
    static constexpr int minStepsDir = 4;
    static constexpr int maxStepsDir = 10;
    const auto m                     = input.size();
    const auto n                     = input[0].size();

    HeatInt_type minLoss = std::numeric_limits<HeatInt_type>::max();

    std::queue<State> q;
    q.emplace(Pos_type{0, 0}, Dir_type{1, 0});  // Right
    q.emplace(Pos_type{0, 0}, Dir_type{0, 1});  // Down

    std::unordered_map<Hash_type, HeatInt_type> seenHashes;

    while (!q.empty()) {
      const auto curr = std::move(q.front());
      q.pop();
      const auto& [i, j]      = curr.pos;
      const auto& [di, dj]    = curr.dir;
      const auto& numSteps    = curr.numSteps;
      const auto& numStepsDir = curr.numStepsDir;
      const auto& heatLoss    = curr.heatLoss;
      if (heatLoss >= minLoss) {
        continue;
      }
      if (i == n - 1 && j == m - 1) {
        if (numStepsDir >= minStepsDir) {
          minLoss = std::min(minLoss, heatLoss);
          continue;
        }
      }
      const auto hash = curr.Hash();
      if (auto seenHashIt = seenHashes.find(hash); seenHashIt != seenHashes.end()) {
        if (seenHashIt->second <= heatLoss) {
          continue;
        }
      }
      seenHashes[hash] = heatLoss;
      for (const auto& dd : ddir) {
        const auto ii = i + dd[0];
        const auto jj = j + dd[1];
        if (Grid::InBounds(ii, jj, n, m)) {
          if (dd[0] == di && dd[1] == dj) {
            if (numStepsDir < maxStepsDir) {
              q.emplace(Pos_type{ii, jj}, Dir_type{di, dj}, numSteps + 1, numStepsDir + 1,
                        heatLoss + input[jj][ii]);
            }
          } else if (dd[0] == -di || dd[1] == -dj) {
            continue;
          } else {
            if (numStepsDir >= minStepsDir) {
              q.emplace(Pos_type{ii, jj}, Dir_type{dd[0], dd[1]}, numSteps + 1, 1,
                        heatLoss + input[jj][ii]);
            }
          }
        }
      }
    }

    return std::to_string(minLoss);
  }
};

};  // namespace AOC2023
