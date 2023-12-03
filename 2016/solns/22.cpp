/////////////////
//// std
/////////////////
#include <limits>
#include <vector>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2016 {

struct day_22 : public Advent_type {
  static constexpr int year                 = 2016;
  static constexpr int date                 = 22;
  const std::vector<std::vector<int>> input = read_lines_vector_regex<int>(
      year, date, "/dev/grid/node-x(\\d+)-y(\\d+)\\s+(\\d+)T\\s+(\\d+)T\\s+(\\d+)T");

  struct Data {
    Data() = default;
    Data(int size, int used, int index, bool isGoal)
        : size(size), used(used), index(index), isGoal(isGoal), isEmpty(used == 0) {}
    int size;
    int used;
    int index;
    bool isGoal;
    bool isEmpty;
  };

  using Grid_type = std::vector<std::vector<Data>>;

  struct State {
    State(const Grid_type& grid, int numSteps, int goalDataX, int goalDataY, int emptyDataX,
          int emptyDataY, int n, int m)
        : grid(grid),
          numSteps(numSteps),
          goalDataX(goalDataX),
          goalDataY(goalDataY),
          emptyDataX(emptyDataX),
          emptyDataY(emptyDataY),
          n(n),
          m(m) {}
    Grid_type grid;
    int numSteps;

    int goalDataX;
    int goalDataY;

    int emptyDataX;
    int emptyDataY;

    int n;
    int m;

    static constexpr auto ddir = Grid::OrthogonalDirections;

    std::vector<State> TestAllDirs(int i, int j) const {
      std::vector<State> newStates;
      for (const auto& dd : ddir) {
        const auto ii = i + dd[0];
        const auto jj = j + dd[1];
        if (Grid::InBounds(ii, jj, n, m)) {
          auto newState = *this;
          auto& dataA   = newState.grid[j][i];
          auto& dataB   = newState.grid[jj][ii];
          if ((dataA.isEmpty && dataB.used <= dataA.size) ||
              (dataB.isEmpty && dataA.used <= dataB.size)) {
            std::swap(dataA, dataB);

            // Must have moved empty data, ~could~ have moved goal.
            newState.emptyDataX = ii;
            newState.emptyDataY = jj;
            if (dataA.isGoal) {
              newState.goalDataX = i;
              newState.goalDataY = j;
            }

            ++newState.numSteps;
            newStates.push_back(newState);
          }
        }
      }
      return newStates;
    }

    std::vector<State> GetNewStates() const {
      std::vector<State> newStates;
      for (const auto& newState : TestAllDirs(emptyDataX, emptyDataY)) {
        newStates.push_back(newState);
      }

      return newStates;
    }

    std::string Hash() const {
      std::string hash;
      for (const auto& v : {goalDataX, goalDataY, emptyDataX, emptyDataY}) {
        hash += std::to_string(v) + "-";
      }
      hash.pop_back();
      return hash;
    }

    bool IsDone() const { return goalDataX == 0 && goalDataY == 0; };

    auto operator<=>(const State& other) const {
      const auto dist      = goalDataX + goalDataY;
      const auto otherDist = other.goalDataX + other.goalDataY;
      return std::tie(dist, numSteps) <=> std::tie(otherDist, other.numSteps);
    }
  };

  std::string part_1() override {
    int ans = 0;
    for (int j = 0; j < input.size(); ++j) {
      const auto& nodeA = input[j];
      if (nodeA[3] == 0) {
        continue;
      }
      for (int i = 0; i < input.size(); ++i) {
        if (i == j) {
          continue;
        }
        const auto& nodeB = input[i];
        ans += static_cast<int>(nodeA[3] <= nodeB[4]);
      }
    }
    return std::to_string(ans);
  }

  std::string part_2() override {
    const auto& n          = (input.back()[0] + 1);
    const auto& m          = (input.back()[1] + 1);
    Grid_type startingGrid = Grid_type(m, std::vector<Data>(n));

    const int goalDataX = n - 1;
    const int goalDataY = 0;
    int emptyDataX      = {};
    int emptyDataY      = {};
    int dataIndex       = 0;
    for (const auto& v : input) {
      const auto& x         = v[0];
      const auto& y         = v[1];
      const auto& size      = v[2];
      const auto& used      = v[3];
      const bool isGoalData = (x == goalDataX) && (y == goalDataY);
      if (used == 0) {
        emptyDataX = x;
        emptyDataY = y;
      }
      startingGrid[y][x] = {size, used, dataIndex++, isGoalData};
    }

    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;
    std::unordered_set<std::string> seenHashes;
    pq.emplace(startingGrid, 0, goalDataX, goalDataY, emptyDataX, emptyDataY, n, m);
    int minSteps = std::numeric_limits<int>::max();
    while (!pq.empty()) {
      const auto currState = std::move(pq.top());
      pq.pop();
      const auto stateHash = currState.Hash();
      if (seenHashes.contains(stateHash)) {
        continue;
      }

      seenHashes.insert(stateHash);

      if (currState.IsDone()) {
        minSteps = std::min(minSteps, currState.numSteps);
        break;
      }

      if (currState.numSteps + 1 >= minSteps) {
        continue;
      }

      for (const auto& newState : currState.GetNewStates()) {
        pq.push(std::move(newState));
      }
    }
    return std::to_string(minSteps);
  }
};

};  // namespace AOC2016
