/////////////////
//// std
/////////////////
#include <algorithm>
#include <cmath>
#include <unordered_map>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2017 {

struct day_21 : public Advent_type {
  static constexpr int year            = 2017;
  static constexpr int date            = 21;
  const std::vector<std::string> input = read_lines<std::string>(year, date);

  using Map_type  = std::unordered_map<int, std::unordered_map<std::string, std::string>>;
  using Row_type  = std::vector<char>;
  using Grid_type = std::vector<Row_type>;

  const Grid_type startingPattern           = {{'.', '#', '.'}, {'.', '.', '#'}, {'#', '#', '#'}};
  static constexpr int numIterationsPartOne = 5;
  static constexpr int numIterationsPartTwo = 18;
  int numPixelsPartOne                      = {};
  int numPixelsPartTwo                      = {};

  Map_type ParseInput() const {
    Map_type patternMap;

    for (const auto& v : input) {
      const auto vv           = Strings::Split(v, " => ");
      const auto size         = (vv[0].size() == 5) ? 2 : 3;
      patternMap[size][vv[0]] = vv[1];
    }

    return patternMap;
  }

  const Map_type patternMap = ParseInput();

  static Grid_type Flip(const Grid_type& input) {
    Grid_type flip = input;
    for (auto& row : flip) {
      std::reverse(row.begin(), row.end());
    }
    return flip;
  }

  static void RotateImpl(Grid_type& input, int n) {
    for (int i = 0; i < n / 2; ++i) {
      for (int j = 0; j < (n + 1) / 2; ++j) {
        auto& a = input[i][j];
        auto& b = input[n - 1 - j][i];
        auto& c = input[n - 1 - i][n - 1 - j];
        auto& d = input[j][n - 1 - i];
        std::swap(a, b);
        std::swap(b, c);
        std::swap(c, d);
      }
    }
  }

  static Grid_type Rotate(const Grid_type& input, int numRots) {
    Grid_type rotate = input;
    const auto n     = rotate.size();
    for (int t = 0; t < numRots; ++t) {
      RotateImpl(rotate, n);
    }
    return rotate;
  }

  static std::vector<Grid_type> GetGrids(const Grid_type& input) {
    std::vector<Grid_type> grids;
    for (bool flip : {false, true}) {
      const auto flipped = (flip) ? Flip(input) : input;
      for (int rot : std::views::iota(0, 4)) {
        const auto rotated = (rot) ? Rotate(flipped, rot) : flipped;
        grids.push_back(rotated);
      }
    }
    return grids;
  }

  static std::vector<Grid_type> SplitSquares(const Grid_type& input, int patternSize) {
    std::vector<Grid_type> squares;
    const auto n = input.size();
    for (int j = 0; j < n; j += patternSize) {
      for (int i = 0; i < n; i += patternSize) {
        Grid_type square;
        for (int jj = 0; jj < patternSize; ++jj) {
          auto row = Row_type();
          for (int ii = 0; ii < patternSize; ++ii) {
            row.push_back(input[j + jj][i + ii]);
          }
          square.push_back(row);
        }
        squares.push_back(square);
      }
    }

    return squares;
  }

  static Grid_type AssembleSquares(const std::vector<Grid_type>& squares) {
    const int numSquares  = std::sqrt(squares.size());
    const int squareN     = squares[0].size();
    const int gridN       = numSquares * squareN;
    auto grid             = Grid_type(gridN, Row_type(gridN));
    const auto indexToPos = [numSquares, squareN](const auto& index) -> std::pair<int, int> {
      return {(index % numSquares) * squareN, (index / numSquares) * squareN};
    };
    for (auto [index, square] : std::views::enumerate(squares)) {
      const auto pos = indexToPos(index);
      for (int j = 0; j < squareN; ++j) {
        const auto jj = pos.second + j;
        for (int i = 0; i < squareN; ++i) {
          const auto ii = pos.first + i;
          grid[jj][ii]  = square[j][i];
        }
      }
    }
    return grid;
  }

  static Grid_type PatternToGrid(std::string_view pattern) {
    Grid_type grid;
    Row_type row;
    for (const auto& c : pattern) {
      if (c == '/') {
        grid.push_back(row);
        row.clear();
      } else {
        row.push_back(c);
      }
    }
    grid.push_back(row);
    return grid;
  }

  static std::string GridToPattern(const Grid_type& grid) {
    std::string pattern = "";
    for (const auto& row : grid) {
      for (const auto& v : row) {
        pattern += v;
      }
      pattern += '/';
    }
    pattern.pop_back();
    return pattern;
  }

  static int countPixels(const Grid_type& grid) {
    return std::accumulate(grid.begin(), grid.end(), 0, [](int sm, const auto& row) {
      return sm + std::count(row.begin(), row.end(), '#');
    });
  }

  void Simulate(Grid_type& grid, int numIterations) {
    for (int t = 0; t < numIterations; ++t) {
      if (t == numIterationsPartOne) {
        numPixelsPartOne = countPixels(grid);
      }

      const auto n           = grid.size();
      const auto patternSize = (n % 2 == 0) ? 2 : 3;
      const auto& patterns   = patternMap.at(patternSize);
      const auto squares     = SplitSquares(grid, patternSize);
      std::vector<Grid_type> newSquares;
      newSquares.reserve(4);
      for (const auto& square : squares) {
        const auto grids = GetGrids(square);
        for (const auto& grid : grids) {
          const auto pattern = GridToPattern(grid);
          if (auto patternsIt = patterns.find(pattern); patternsIt != patterns.end()) {
            newSquares.push_back(PatternToGrid(patternsIt->second));
            break;
          }
        }
      }
      grid = AssembleSquares(newSquares);
    }
    numPixelsPartTwo = countPixels(grid);
  }

  std::string part_1() override {
    auto grid = startingPattern;
    Simulate(grid, numIterationsPartTwo);
    return std::to_string(numPixelsPartOne);
  }

  std::string part_2() override { return std::to_string(numPixelsPartTwo); }
};

};  // namespace AOC2017
