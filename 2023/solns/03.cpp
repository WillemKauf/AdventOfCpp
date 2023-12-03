/////////////////
//// std
/////////////////
#include <functional>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2023 {

struct day_03 : public Advent_type {
  static constexpr int year            = 2023;
  static constexpr int date            = 3;
  const std::vector<std::string> input = read_lines<std::string>(year, date);

  struct Number {
    Number(int value, int index) : value(value), index(index) {}
    int value;
    int index;
  };

  using Pair_type         = std::pair<int, int>;
  using HashFunction_type = std::function<int(const Pair_type&)>;
  using NumberMap_type    = std::unordered_map<Pair_type, Number, HashFunction_type>;
  using SymbolMap_type    = std::unordered_map<Pair_type, char, HashFunction_type>;
  NumberMap_type numberMap;
  SymbolMap_type symbolMap;

  static constexpr auto ddir = Grid::DiagonalDirections;

  std::string part_1() override {
    const auto m = input.size();
    const auto n = input[0].size();

    const auto hashFunc    = [n](const Pair_type& p) { return p.second * n + p.first; };
    const auto bucketCount = 10;
    numberMap              = NumberMap_type(bucketCount, hashFunc);
    symbolMap              = SymbolMap_type(bucketCount, hashFunc);

    int numNums = 0;
    int ans     = 0;
    for (auto [j, row] : std::views::enumerate(input)) {
      std::string currNum = {};
      bool isPartNumber   = false;
      for (auto [i, c] : std::views::enumerate(row)) {
        if (std::isdigit(c)) {
          currNum += c;
          for (const auto& dd : ddir) {
            const auto ii = i + dd[0];
            const auto jj = j + dd[1];
            if(Grid::InBounds(ii, jj, n, m)){
              if (!std::isdigit(input[jj][ii]) && input[jj][ii] != '.') {
                isPartNumber = true;
              }
            }
          }
        } else {
          if (c != '.') {
            symbolMap.emplace(Pair_type{i, j}, c);
          }
          if (!currNum.empty()) {
            if (isPartNumber) {
              ans += std::stoi(currNum);
              isPartNumber = false;
            }
            for (int ii = i - currNum.size(); ii < i; ++ii) {
              numberMap.insert_or_assign(Pair_type{ii, j}, Number{std::stoi(currNum), numNums});
            }
            ++numNums;
            currNum.clear();
          }
        }
      }
      if (!currNum.empty()) {
        if (isPartNumber) {
          ans += std::stoi(currNum);
        }
        for (int i = n - currNum.size(); i < n; ++i) {
          numberMap.insert_or_assign(Pair_type{i, j}, Number{std::stoi(currNum), numNums});
        }
        ++numNums;
      }
    }
    return std::to_string(ans);
  }

  std::string part_2() override {
    const auto m = input.size();
    const auto n = input[0].size();
    int ans      = 0;
    return std::to_string(
        std::accumulate(symbolMap.begin(), symbolMap.end(), 0, [&](int sm, const auto& p) {
          const auto& k = p.first;
          const auto& v = p.second;
          if (v == '*') {
            const auto& i = k.first;
            const auto& j = k.second;
            int num       = 1;
            std::unordered_set<int> seenIndices;
            for (const auto& dd : ddir) {
              const auto ii = i + dd[0];
              const auto jj = j + dd[1];
              if(Grid::InBounds(ii, jj, n, m)){
                const auto newPos = Pair_type{ii, jj};
                if (auto numberMapIt = numberMap.find(newPos); numberMapIt != numberMap.end()) {
                  const auto& partNumber  = numberMapIt->second.value;
                  const auto& numberIndex = numberMapIt->second.index;
                  if (!seenIndices.contains(numberIndex)) {
                    num *= partNumber;
                    seenIndices.insert(numberIndex);
                  }
                }
              }
            }
            sm += (seenIndices.size() == 2) ? num : 0;
          }
          return sm;
        }));
  }
};

};  // namespace AOC2023
