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

struct day_14 : public Advent_type {
  static constexpr int year            = 2023;
  static constexpr int date            = 14;
  const std::vector<std::string> input = read_lines<std::string>(year, date);

  using Grid_type = std::vector<std::string>;

  int ComputeLoad(const Grid_type& grid) const {
    int load     = 0;
    const auto m = grid.size();
    for (const auto& [j, row] : std::views::enumerate(grid)) {
      for (const auto& c : row) {
        if (c == 'O') {
          load += m - j;
        }
      }
    }
    return load;
  }

  enum class DirectionType {
    NORTH,
    WEST,
    SOUTH,
    EAST
  };

  template <DirectionType directionType>
  static constexpr bool isPositive =
      (directionType == DirectionType::NORTH || directionType == DirectionType::WEST);

  template <bool isPos>
  void TiltPlatformImpl(Grid_type& grid) const {
    auto newGrid = grid;
    if constexpr (isPos) {
      for (const auto& [j, row] : std::views::enumerate(grid)) {
        auto& newRow = newGrid[j];
        int p        = 0;
        int numEmpty = 0;
        for (const auto& [i, c] : std::views::enumerate(row)) {
          if (c == 'O') {
            newRow[p++] = c;
          } else if (c == '.') {
            ++numEmpty;
          } else if (c == '#') {
            for (; numEmpty; --numEmpty) {
              newRow[p++] = '.';
            }
            ++p;
          }
        }
        for (; numEmpty; --numEmpty) {
          newRow[p++] = '.';
        }
      }
    } else {
      const auto n = grid[0].size();
      for (const auto& [j, row] : std::views::enumerate(grid)) {
        auto& newRow = newGrid[j];
        int p        = n - 1;
        int numEmpty = 0;
        for (const auto& [i, c] : std::views::enumerate(std::views::reverse(row))) {
          if (c == 'O') {
            newRow[p--] = c;
          } else if (c == '.') {
            ++numEmpty;
          } else if (c == '#') {
            for (; numEmpty; --numEmpty) {
              newRow[p--] = '.';
            }
            --p;
          }
        }
        for (; numEmpty; --numEmpty) {
          newRow[p--] = '.';
        }
      }
    }
    grid = std::move(newGrid);
  }

  template <DirectionType directionType>
  void TiltPlatform(Grid_type& grid) const {
    if constexpr (directionType == DirectionType::NORTH || directionType == DirectionType::SOUTH) {
      auto newGrid = Algorithm::TransposeGrid(grid);
      TiltPlatformImpl<isPositive<directionType>>(newGrid);
      grid = std::move(Algorithm::TransposeGrid(newGrid));
    } else {
      TiltPlatformImpl<isPositive<directionType>>(grid);
    }
  }

  std::string part_1() override {
    auto grid = input;
    TiltPlatform<DirectionType::NORTH>(grid);
    return std::to_string(ComputeLoad(grid));
  }

  std::string part_2() override {
    static constexpr auto numCycles = 1000000000;
    auto grid                       = input;
    std::unordered_map<std::string, std::pair<int, int>> seenHashes;
    const auto HashGrid = [](const auto& grid) {
      std::string hash;
      for (const auto& row : grid) {
        hash += row;
      }
      return hash;
    };
    for (int n = 0; n < numCycles; ++n) {
      const auto hash = HashGrid(grid);
      if (auto seenHashIt = seenHashes.find(hash); seenHashIt != seenHashes.end()) {
        const auto prevSeen   = seenHashIt->second.first;
        const auto cycleSize  = n - prevSeen;
        const auto finalCycle = (numCycles - prevSeen) % cycleSize + prevSeen;
        const auto finalLoad =
            std::find_if(seenHashes.begin(), seenHashes.end(), [finalCycle](const auto& p) {
              return p.second.first == finalCycle;
            })->second.second;
        return std::to_string(finalLoad);
      }
      seenHashes.insert_or_assign(hash, std::pair{n, ComputeLoad(grid)});
      TiltPlatform<DirectionType::NORTH>(grid);
      TiltPlatform<DirectionType::WEST>(grid);
      TiltPlatform<DirectionType::SOUTH>(grid);
      TiltPlatform<DirectionType::EAST>(grid);
    }
    return std::to_string(ComputeLoad(grid));
  }
};

};  // namespace AOC2023
