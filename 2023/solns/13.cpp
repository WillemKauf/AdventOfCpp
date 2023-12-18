/////////////////
//// std
/////////////////
#include <stack>

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

  using Grid_type  = std::vector<std::string>;
  using Grids_type = std::vector<Grid_type>;

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

  enum class AxisType {
    HORIZONTAL,
    VERTICAL
  };

  using Hash_type = uint32_t;

  template <typename T>
  Hash_type Hash(const T& t) const {
    Hash_type hash = {};
    for(const auto& [index, c] : std::views::enumerate(t)){
      const Hash_type v = (c == '.') ? 0 : 1;
      hash |= (v << index);
    }
    return hash;
  }

  /*
  xxxxx0123443210
  0123443210yyyyy
  because if we had xxxxx0123443210yyyyy we wouldn't have a perfect match.
  the length of x/y section shouldn't matter, even if x section includes some symmetrical sections.
  i.e
  xxxxxx |
  012210 | 34566543
  */
  template <AxisType axisType, bool partTwo = false>
  std::optional<std::size_t> GetReflectionImpl(const Grid_type& grid) const {
    std::stack<Hash_type> st;
    const auto n = grid.size();
    const auto compareFunctionEq = [](const auto& a, const auto& b) { return a == b; };
    const auto compareFunction = [](const auto& a, const auto& b) {
      int xorResult    = a ^ b;
      int countSetBits = 0;
      while (xorResult) {
        countSetBits += xorResult & 1;
        xorResult >>= 1;
      }
      return countSetBits == 1;
    };
    for (std::size_t p = 0; p < n; ++p) {
      const auto hashP = Hash(grid[p]);
      if (!st.empty() && (compareFunctionEq(hashP,st.top()) || compareFunction(hashP,st.top()))) {
      //if (!st.empty() && (hashP == st.top())) {
        bool oneWrong = true;
        const auto currPos = p;
        auto stCpy         = st;
        auto hash          = hashP;
        for (; (!stCpy.empty()) && (p < n); ++p) {
          hash = Hash(grid[p]);
          //PartOne
          //if(!compareFunctionEq(hash, stCpy.top())){
          //  break;
          //}
          if(!compareFunctionEq(hash, stCpy.top())){
            if(oneWrong && compareFunction(hash, stCpy.top())){
              oneWrong = false;
            }else{
              break;
            }
          }
          st.push(hash);
          stCpy.pop();
        }
        const auto dist = p - currPos;
        if ((p == n) || (currPos - dist == 0)) {
          if(!oneWrong){
            return currPos;
          }
        }
        st.push(hash);
      } else {
        st.push(hashP);
      }
    }
    return std::nullopt;
  }

  template <AxisType axisType, bool partTwo = false>
  auto GetReflection(const Grid_type& grid) const {
    if constexpr (axisType == AxisType::HORIZONTAL) {
      return GetReflectionImpl<axisType, partTwo>(grid);
    } else {
      return GetReflectionImpl<axisType, partTwo>(Algorithm::TransposeGrid(grid));
    }
  }

  std::unordered_map<int, std::pair<int, AxisType>> mp;

  std::string part_1() override {
    int sm = 0;
    for (const auto& [index, grid] : std::views::enumerate(grids)) {
      const auto vReflOpt = GetReflection<AxisType::VERTICAL>(grid);
      const auto hReflOpt = GetReflection<AxisType::HORIZONTAL>(grid);
      if (vReflOpt.has_value()) {
        mp[index] = {vReflOpt.value(), AxisType::VERTICAL};
        sm += vReflOpt.value();
      } else if (hReflOpt.has_value()) {
        mp[index] = {hReflOpt.value(), AxisType::HORIZONTAL};
        sm += 100 * hReflOpt.value();
      } else {
        for(const auto& row : grid){
          std::cout << row << '\n';
        }
        std::cout << '\n';
        throw std::runtime_error("No value for either reflection");
      }
    }
    return std::to_string(sm);
  }

  std::string part_2() override {
    int sm = 0;
    for (const auto& [index, grid] : std::views::enumerate(grids)) {
      const auto vReflOpt = GetReflection<AxisType::VERTICAL>(grid);
      const auto hReflOpt = GetReflection<AxisType::HORIZONTAL>(grid);
      if (vReflOpt.has_value()) {
        mp[index] = {vReflOpt.value(), AxisType::VERTICAL};
        sm += vReflOpt.value();
      } else if (hReflOpt.has_value()) {
        mp[index] = {hReflOpt.value(), AxisType::HORIZONTAL};
        sm += 100 * hReflOpt.value();
      } else {
        throw std::runtime_error("No value for either reflection");
      }
    }
    return std::to_string(sm);
  }
};

};  // namespace AOC2023
