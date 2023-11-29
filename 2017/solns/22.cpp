/////////////////
//// std
/////////////////
#include <complex>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2017 {

struct day_22 : public Advent_type {
  static constexpr int year            = 2017;
  static constexpr int date            = 22;
  const std::vector<std::string> input = read_lines<std::string>(year, date);

  using Complex_type = std::complex<int>;

  enum class NodeType {
    CLEAN,
    WEAKENED,
    INFECTED,
    FLAGGED
  };

  using Grid_type = std::unordered_map<std::string, NodeType>;

  struct State {
    State(const Complex_type& pos, const Complex_type& dir) : pos(pos), dir(dir) {}
    Complex_type pos;
    Complex_type dir;
  };

  static auto hashPos(int i, int j) { return std::to_string(i) + "," + std::to_string(j); }

  Grid_type ParseInput() const {
    Grid_type grid;
    for (auto [j, row] : std::views::enumerate(input)) {
      for (auto [i, v] : std::views::enumerate(row)) {
        const auto n        = (v == '.') ? NodeType::CLEAN : NodeType::INFECTED;
        grid[hashPos(i, j)] = n;
      }
    }
    return grid;
  }

  const Grid_type inputGrid = ParseInput();

  std::string part_1() override {
    static constexpr int numBursts = 10000;
    auto grid                      = inputGrid;
    const int m                    = input.size();
    const int n                    = input[0].size();
    auto state                     = State(Complex_type{n / 2, m / 2}, Complex_type{0, 1});
    int numInfections              = 0;
    for (int t = 0; t < numBursts; ++t) {
      auto& d = state.dir;
      auto& c = grid[hashPos(state.pos.real(), state.pos.imag())];
      switch (c) {
        case NodeType::INFECTED:
          d *= Complex_type{0, -1};
          c = NodeType::CLEAN;
          break;
        case NodeType::CLEAN:
          d *= Complex_type{0, 1};
          c = NodeType::INFECTED;
          ++numInfections;
          break;
        default:
          throw std::runtime_error("Invalid NodeType");
      }
      state.pos = Complex_type{state.pos.real() + d.real(), state.pos.imag() - d.imag()};
    }
    return std::to_string(numInfections);
  }

  std::string part_2() override {
    static constexpr int numBursts = 10000000;
    auto grid                      = inputGrid;
    const int m                    = input.size();
    const int n                    = input[0].size();
    auto state                     = State(Complex_type{n / 2, m / 2}, Complex_type{0, 1});
    int numInfections              = 0;
    for (int t = 0; t < numBursts; ++t) {
      auto& d = state.dir;
      auto& c = grid[hashPos(state.pos.real(), state.pos.imag())];
      switch (c) {
        case NodeType::CLEAN:
          d *= Complex_type{0, 1};
          c = NodeType::WEAKENED;
          break;
        case NodeType::WEAKENED:
          c = NodeType::INFECTED;
          ++numInfections;
          break;
        case NodeType::INFECTED:
          d *= Complex_type{0, -1};
          c = NodeType::FLAGGED;
          break;
        case NodeType::FLAGGED:
          d *= Complex_type{-1, 0};
          c = NodeType::CLEAN;
          break;
        default:
          throw std::runtime_error("Invalid NodeType");
      }
      state.pos = Complex_type{state.pos.real() + d.real(), state.pos.imag() - d.imag()};
    }
    return std::to_string(numInfections);
  }
};

};  // namespace AOC2017
