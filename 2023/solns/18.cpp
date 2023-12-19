/////////////////
//// std
/////////////////
#include <cstdint>
#include <queue>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2023 {

struct day_18 : public Advent_type {
  static constexpr int year = 2023;
  static constexpr int date = 18;
  const std::vector<std::vector<std::string>> input =
      read_lines_vector_regex<std::string>(year, date, "(\\w) (\\d+) \\((.*)\\)");

  static constexpr auto ddir = Grid::OrthogonalDirections;

  using PosInt_type = int16_t;
  using Pos_type    = std::pair<PosInt_type, PosInt_type>;
  using Hash_type   = int32_t;
  using Row_type    = std::vector<bool>;
  using Grid_type   = std::vector<Row_type>;

  enum class DirectionType {
    L,
    R,
    U,
    D
  };

  struct Instruct {
    Instruct(DirectionType direction, int numSteps, std::string colorHex)
        : direction(direction), numSteps(numSteps), colorHex(colorHex) {}
    DirectionType direction;
    int numSteps;
    std::string colorHex;
  };

  struct HashPos {
    Hash_type operator()(const Pos_type& p) const {
      static constexpr auto sizeOfInt = sizeof(PosInt_type) * 8;
      Hash_type hash                  = {};
      hash |= p.first;
      hash |= (static_cast<Hash_type>(p.second) << sizeOfInt);
      return hash;
    }
  };

  struct State {
    State(const Pos_type& pos, int numSteps) : pos(pos), numSteps(numSteps) {}
    Pos_type pos;
    int numSteps;
  };

  std::vector<Instruct> ParseInput() const {
    std::vector<Instruct> instructs;
    instructs.reserve(input.size());
    for (const auto& v : input) {
      const auto direction = [&](const auto& dirStr) {
        if (dirStr == "L") {
          return DirectionType::L;
        } else if (dirStr == "R") {
          return DirectionType::R;
        } else if (dirStr == "U") {
          return DirectionType::U;
        } else if (dirStr == "D") {
          return DirectionType::D;
        } else {
          throw std::runtime_error("Invalid dirStr");
        }
      }(v[0]);
      const auto numSteps = std::stoi(v[1]);
      instructs.emplace_back(direction, numSteps, v[2].substr(1));
    }

    return instructs;
  }

  const std::vector<Instruct> instructInput = ParseInput();

  std::pair<Grid_type, uint64_t> GetGrid() const {
    Pos_type pos = {0, 0};

    PosInt_type minX = 0;
    PosInt_type maxX = 0;
    PosInt_type minY = 0;
    PosInt_type maxY = 0;

    std::unordered_set<Pos_type, HashPos> dugHoles = {pos};

    for (const auto& instruct : instructInput) {
      const auto& numSteps = instruct.numSteps;
      switch (instruct.direction) {
        case DirectionType::L: {
          for (int i = 0; i < numSteps; ++i) {
            --pos.first;
            dugHoles.insert(pos);
          }
          break;
        }
        case DirectionType::R: {
          for (int i = 0; i < numSteps; ++i) {
            ++pos.first;
            dugHoles.insert(pos);
          }
          break;
        }
        case DirectionType::U: {
          for (int j = 0; j < numSteps; ++j) {
            --pos.second;
            dugHoles.insert(pos);
          }
          break;
        }
        case DirectionType::D: {
          for (int j = 0; j < numSteps; ++j) {
            ++pos.second;
            dugHoles.insert(pos);
          }
          break;
        }
        default:
          throw std::runtime_error("Invalid DirectionType");
      }
      minX = std::min(minX, pos.first);
      maxX = std::max(maxX, pos.first);
      minY = std::min(minY, pos.second);
      maxY = std::max(maxY, pos.second);
    }

    uint64_t lagoonSize = dugHoles.size();
    const auto m        = (maxY - minY) + 1;
    const auto n        = (maxX - minX) + 1;
    auto grid           = Grid_type(m, Row_type(n));
    for (const auto& dugHole : dugHoles) {
      const auto i = dugHole.first - minX;
      const auto j = dugHole.second - minY;
      grid[j][i]   = true;
    }
    return {grid, lagoonSize};
  }

  uint64_t Solve() const {
    auto inputGrid   = GetGrid();
    auto& grid       = inputGrid.first;
    auto& lagoonSize = inputGrid.second;
    const auto m     = grid.size();
    const auto n     = grid[0].size();

    std::unordered_set<Pos_type, HashPos> seen;
    for (const auto& [j, row] : std::views::enumerate(grid)) {
      for (const auto& [i, c] : std::views::enumerate(row)) {
        int numInterior = 0;
        bool isInterior = true;
        if (!c) {
          std::queue<State> q;
          q.emplace(Pos_type{i, j}, 0);

          while (!q.empty()) {
            const auto curr = std::move(q.front());
            q.pop();
            if (seen.contains(curr.pos)) {
              continue;
            }
            seen.insert(curr.pos);
            const auto& i        = curr.pos.first;
            const auto& j        = curr.pos.second;
            const auto& numSteps = curr.numSteps;
            ++numInterior;
            for (const auto& dd : ddir) {
              const auto ii = i + dd[0];
              const auto jj = j + dd[1];
              if (Grid::InBounds(ii, jj, n, m)) {
                if (!grid[jj][ii]) {
                  q.emplace(Pos_type{ii, jj}, numSteps + 1);
                }
              } else {
                isInterior = false;
              }
            }
          }
        }
        if (isInterior) {
          lagoonSize += numInterior;
        }
      }
    }

    return lagoonSize;
  }

  std::string part_1() override { return std::to_string(Solve()); }

  std::string part_2() override {
    uint64_t I                                            = 0;
    uint64_t P                                            = 0;
    uint64_t x                                            = 0;
    uint64_t y                                            = 0;
    const std::unordered_map<char, Pos_type> directionMap = {
        {'0', {1, 0}}, {'1', {0, 1}}, {'2', {-1, 0}}, {'3', {0, -1}}};
    std::for_each(instructInput.begin(), instructInput.end(), [&](const auto& instruct) {
      const auto hexSubStr = instruct.colorHex.substr(0, instruct.colorHex.size() - 1);
      const auto numSteps  = Algorithm::HexToDec(hexSubStr);
      const auto& dd       = directionMap.at(instruct.colorHex[instruct.colorHex.size() - 1]);
      const auto dx        = dd.first * numSteps;
      const auto dy        = dd.second * numSteps;
      x += dx;
      y += dy;
      I += x * dy;
      P += numSteps;
    });
    const auto A = I + P / 2 + 1;
    return std::to_string(A);
  }
};

};  // namespace AOC2023
