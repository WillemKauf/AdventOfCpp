/////////////////
//// std
/////////////////

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"
#include "grid.h"

namespace AOC2017 {

struct day_14 : public Advent_type {
  static constexpr int year = 2017;
  static constexpr int date = 14;
  const std::string input   = read_lines<std::string>(year, date)[0];

  static constexpr int gridSize = 128;
  using Grid_type               = std::array<std::array<int, gridSize>, gridSize>;

  Grid_type ParseInput() const {
    Grid_type grid;
    static constexpr int bitSize = 4;
    for (int j = 0; j < gridSize; ++j) {
      const auto hashStr = input + "-" + std::to_string(j);
      const auto hash    = AOC2017::day_10::RunKnotHash(hashStr);
      int i              = 0;
      for (const auto& v : hash) {
        const auto b = Algorithm::HexToBin<bitSize>(v);
        for (int bi = 0; bi < bitSize; ++bi) {
          grid[j][i++] = b[bi];
        }
      }
    }
    return grid;
  }

  const Grid_type grid = ParseInput();

  std::string part_1() override {
    return std::to_string(std::accumulate(grid.begin(), grid.end(), 0, [](int sm, const auto& row) {
      return sm + std::count(row.begin(), row.end(), 1);
    }));
  }

  std::string part_2() override {
    static constexpr auto ddir = Grid::OrthogonalDirections;
    std::unordered_set<int> seen;
    int numRegions      = 0;
    const auto hashFunc = [&](const auto& i, const auto& j) { return i + j * gridSize; };
    for (int j = 0; j < gridSize; ++j) {
      for (int i = 0; i < gridSize; ++i) {
        if (grid[j][i] == 0) {
          continue;
        }
        const auto hash = hashFunc(i, j);
        if (seen.contains(hash)) {
          continue;
        }
        seen.insert(hash);
        ++numRegions;
        std::queue<std::pair<int, int>> q;
        q.emplace(i, j);
        while (!q.empty()) {
          const auto currPos = std::move(q.front());
          q.pop();
          for (const auto& dd : ddir) {
            const auto ii = currPos.first + dd[0];
            const auto jj = currPos.second + dd[1];
            if (Grid::InBounds(ii, jj, gridSize, gridSize)) {
              if (grid[jj][ii] == 0) {
                continue;
              }
              const auto hash = hashFunc(ii, jj);
              if (seen.contains(hash)) {
                continue;
              }
              seen.insert(hash);
              q.emplace(ii, jj);
            }
          }
        }
      }
    }
    return std::to_string(numRegions);
  }
};

};  // namespace AOC2017
