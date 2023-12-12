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

struct day_11 : public Advent_type {
  static constexpr int year            = 2023;
  static constexpr int date            = 11;
  const std::vector<std::string> input = read_lines<std::string>(year, date);

  using Int_type = int64_t;
  using Set_type = std::unordered_set<Int_type>;
  using Vec_type = std::vector<std::pair<Int_type, Int_type>>;

  struct Sizes {
    Set_type rowSizes;
    Set_type colSizes;
    Vec_type galaxySet;
  };

  Sizes ParseInput() const {
    const auto m = input.size();
    const auto n = input[0].size();
    Set_type rowSizes;
    Set_type colSizes;
    Vec_type galaxySet;
    for (const auto& [j, row] : std::views::enumerate(input)) {
      for (const auto& [i, c] : std::views::enumerate(row)) {
        if (c == '#') {
          rowSizes.insert(j);
          colSizes.insert(i);
          galaxySet.emplace_back(i, j);
        }
      }
    }
    return {rowSizes, colSizes, galaxySet};
  }

  const Sizes inputSizes    = ParseInput();
  const Set_type& rowSizes  = inputSizes.rowSizes;
  const Set_type& colSizes  = inputSizes.colSizes;
  const Vec_type& galaxySet = inputSizes.galaxySet;

  template <Int_type scaleFactor>
  Vec_type CreateNewGalaxySet() const {
    Vec_type newGalaxySet;
    newGalaxySet.reserve(galaxySet.size());
    const auto m = input.size();
    const auto n = input[0].size();
    Int_type jj  = 0;
    for (const auto& [j, row] : std::views::enumerate(input)) {
      const Int_type dj = (rowSizes.contains(j) ? 1 : scaleFactor);
      Int_type ii       = 0;
      for (const auto& [i, c] : std::views::enumerate(row)) {
        const Int_type di = (colSizes.contains(i) ? 1 : scaleFactor);
        if (c == '#') {
          newGalaxySet.emplace_back(ii, jj);
        }
        ii += di;
      }
      jj += dj;
    }
    return newGalaxySet;
  }

  template <Int_type scaleFactor>
  Int_type GetSumLengths() const {
    const auto newGalaxySet       = CreateNewGalaxySet<scaleFactor>();
    const auto galaxyCombinations = Algorithm::GetCombinations(newGalaxySet, 2);
    return std::accumulate(
        galaxyCombinations.begin(), galaxyCombinations.end(), Int_type{0},
        [](Int_type sm, const auto& galaxyCombination) {
          const Int_type dist = std::abs(galaxyCombination[0].first - galaxyCombination[1].first) +
                                std::abs(galaxyCombination[0].second - galaxyCombination[1].second);
          return sm + dist;
        });
  }

  std::string part_1() override {
    static constexpr Int_type scaleFactor = 2;
    return std::to_string(GetSumLengths<scaleFactor>());
  }

  std::string part_2() override {
    static constexpr Int_type scaleFactor = 1000000;
    return std::to_string(GetSumLengths<scaleFactor>());
  }
};

};  // namespace AOC2023
