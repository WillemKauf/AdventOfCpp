/////////////////
//// std
/////////////////
#include <cstdint>
#include <limits>
#include <queue>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2016 {

struct day_24 : public Advent_type {
  static constexpr int year = 2016;
  static constexpr int date = 24;

  using Grid_type      = std::vector<std::vector<char>>;
  const Grid_type grid = read_lines_vector(year, date);

  using UniqueCoords_type = std::unordered_map<int, std::pair<int, int>>;
  using DistanceMap_type  = std::unordered_map<int, std::unordered_map<int, int>>;

  UniqueCoords_type ParseUniqueCoords() const {
    UniqueCoords_type numberMap = {};
    for (auto [j, row] : std::views::enumerate(grid)) {
      for (auto [i, v] : std::views::enumerate(row)) {
        if (std::isdigit(v)) {
          numberMap[v - '0'] = {i, j};
        }
      }
    }
    return numberMap;
  }

  const UniqueCoords_type numberCoords = ParseUniqueCoords();

  DistanceMap_type ParseDistanceMap() const {
    struct State {
      State(int i, int j, int numSteps) : i(i), j(j), numSteps(numSteps) {}
      int i;
      int j;
      int numSteps;
    };

    DistanceMap_type distanceMap;
    const auto m = grid.size();
    const auto n = grid[0].size();
    for (const auto& [num, pos] : numberCoords) {
      std::queue<State> q          = {};
      std::unordered_set<int> seen = {};
      q.emplace(pos.first, pos.second, 0);
      while (!q.empty()) {
        const auto currState = std::move(q.front());
        q.pop();

        const auto i        = currState.i;
        const auto j        = currState.j;
        const auto numSteps = currState.numSteps;

        const auto posHash = i + j * n;

        if (seen.contains(posHash)) {
          continue;
        }

        seen.insert(posHash);

        if (std::isdigit(grid[j][i]) && num != grid[j][i] - '0') {
          distanceMap[num][grid[j][i] - '0'] = numSteps;
        }

        for (const auto& dd : ddir) {
          const auto ii = i + dd[0];
          const auto jj = j + dd[1];
          if (Grid::InBounds(ii, jj, n, m)) {
            if (grid[jj][ii] != '#') {
              q.emplace(ii, jj, numSteps + 1);
            }
          }
        }
      }
    }

    return distanceMap;
  }

  static constexpr auto ddir = Grid::OrthogonalDirections;

  int minStepsPartTwo = std::numeric_limits<int>::max();

  std::string part_1() override {
    struct State {
      State(int currNumber, int visited, int numSteps)
          : currNumber(currNumber), visited(visited), numSteps(numSteps) {}
      int currNumber;
      uint8_t visited;
      int numSteps;

      auto operator<=>(const State& other) const {
        return std::tie(visited, other.numSteps) <=> std::tie(other.visited, numSteps);
      }
    };

    const auto distanceMap = ParseDistanceMap();
    std::priority_queue<State> pq;

    uint8_t allVisited = 0;
    for (const auto& [number, _] : numberCoords) {
      const auto shift = (1 << number);
      allVisited |= shift;
    }

    const auto startingNumber = 0;
    const auto shift          = (1 << startingNumber);
    pq.emplace(startingNumber, shift, 0);

    int minSteps = minStepsPartTwo;
    while (!pq.empty()) {
      const auto currState = std::move(pq.top());
      pq.pop();

      if (currState.visited == allVisited) {
        minSteps        = std::min(minSteps, currState.numSteps);
        minStepsPartTwo = std::min(minStepsPartTwo,
                                   currState.numSteps + distanceMap.at(currState.currNumber).at(0));
        continue;
      }

      for (const auto& [num, dist] : distanceMap.at(currState.currNumber)) {
        const auto shift = (1 << num);
        if (currState.visited & shift) {
          continue;
        }
        const auto newSteps = currState.numSteps + dist;
        if (newSteps <= minSteps) {
          pq.emplace(num, currState.visited | shift, newSteps);
        }
      }
    }

    return std::to_string(minSteps);
  }

  std::string part_2() override { return std::to_string(minStepsPartTwo); }
};

};  // namespace AOC2016
