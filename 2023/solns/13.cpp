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

struct day_13 : public Advent_type {
  static constexpr int year            = 2023;
  static constexpr int date            = 13;
  const std::vector<std::string> input = read_lines<std::string>(year, date);

  using Hash_type  = uint32_t;
  using Grid_type  = std::vector<std::string>;
  using Grids_type = std::vector<Grid_type>;

  enum class AxisType {
    HORIZONTAL,
    VERTICAL
  };

  Grids_type ParseInput() const {
    Grids_type grids;
    Grid_type currGrid;
    for (const auto& row : input) {
      if (row.empty()) {
        grids.push_back(currGrid);
        currGrid.clear();
      } else {
        currGrid.push_back(row);
      }
    }
    if (!currGrid.empty()) {
      grids.push_back(currGrid);
    }
    return grids;
  }

  const Grids_type grids = ParseInput();

  template <typename T>
  Hash_type Hash(const T& t) const {
    Hash_type hash = {};
    for (const auto& [index, c] : std::views::enumerate(t)) {
      const Hash_type v = (c == '.') ? 0 : 1;
      hash |= (v << index);
    }
    return hash;
  }

  static bool compareFunctionEq(Hash_type a, Hash_type b) { return a == b; }

  static bool compareFunctionOffByOne(Hash_type a, Hash_type b) {
    Hash_type xorResult  = a ^ b;
    uint8_t countSetBits = 0;
    while (xorResult) {
      countSetBits += xorResult & 1;
      xorResult >>= 1;
    }
    return (countSetBits == 1);
  }

  template <bool partTwo = false>
  bool TestReflection(std::size_t p, const Grid_type& grid, bool hasOffByOne = false) const {
    const auto n = grid.size();
    int pOne     = p - 2;
    int pTwo     = p + 1;
    if (pOne < 0 || pTwo >= n) {
      if constexpr (partTwo) {
        return hasOffByOne;
      } else {
        return true;
      }
    }

    for (; pOne >= 0 && pTwo < n; --pOne, ++pTwo) {
      const auto hashA = Hash(grid[pOne]);
      const auto hashB = Hash(grid[pTwo]);
      if constexpr (partTwo) {
        if (!compareFunctionEq(hashA, hashB)) {
          if (!hasOffByOne && compareFunctionOffByOne(hashA, hashB)) {
            hasOffByOne = true;
          } else {
            return false;
          }
        }
      } else {
        if (!compareFunctionEq(hashA, hashB)) {
          return false;
        }
      }
    }

    if constexpr (partTwo) {
      return hasOffByOne;
    } else {
      return true;
    }
  }

  template <bool partTwo = false>
  std::optional<std::size_t> GetReflectionImpl(const Grid_type& grid) const {
    const auto n = grid.size();
    for (std::size_t p = 1; p < n; ++p) {
      const auto hashA = Hash(grid[p - 1]);
      const auto hashB = Hash(grid[p]);
      if constexpr (partTwo) {
        const auto hasOffByOne = compareFunctionOffByOne(hashA, hashB);
        if (compareFunctionEq(hashA, hashB) || hasOffByOne) {
          if (TestReflection<partTwo>(p, grid, hasOffByOne)) {
            return p;
          }
        }
      } else {
        if (compareFunctionEq(hashA, hashB)) {
          if (TestReflection<partTwo>(p, grid)) {
            return p;
          }
        }
      }
    }
    return std::nullopt;
  }

  template <AxisType axisType, bool partTwo = false>
  auto GetReflection(const Grid_type& grid) const {
    if constexpr (axisType == AxisType::HORIZONTAL) {
      return GetReflectionImpl<partTwo>(grid);
    } else {
      return GetReflectionImpl<partTwo>(Algorithm::TransposeGrid(grid));
    }
  }

  std::string part_1() override {
    int sm = 0;
    for (const auto& [index, grid] : std::views::enumerate(grids)) {
      const auto vReflOpt = GetReflection<AxisType::VERTICAL>(grid);
      if (vReflOpt.has_value()) {
        sm += vReflOpt.value();
      } else {
        const auto hReflOpt = GetReflection<AxisType::HORIZONTAL>(grid);
        if (hReflOpt.has_value()) {
          sm += 100 * hReflOpt.value();
        } else {
          throw std::runtime_error("No value for either reflection");
        }
      }
    }
    return std::to_string(sm);
  }

  std::string part_2() override {
    int sm = 0;
    for (const auto& [index, grid] : std::views::enumerate(grids)) {
      const auto vReflOpt = GetReflection<AxisType::VERTICAL, true>(grid);
      if (vReflOpt.has_value()) {
        sm += vReflOpt.value();
      } else {
        const auto hReflOpt = GetReflection<AxisType::HORIZONTAL, true>(grid);
        if (hReflOpt.has_value()) {
          sm += 100 * hReflOpt.value();
        } else {
          throw std::runtime_error("No value for either reflection");
        }
      }
    }
    return std::to_string(sm);
  }
};

};  // namespace AOC2023
