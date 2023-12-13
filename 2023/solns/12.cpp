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

struct day_12 : public Advent_type {
  static constexpr int year            = 2023;
  static constexpr int date            = 12;
  const std::vector<std::string> input = read_lines<std::string>(year, date);

  using Int_type    = uint64_t;
  using Short_type  = uint16_t;
  using Record_type = std::pair<std::string, std::vector<int>>;
  using Vec_type    = std::vector<Record_type>;
  using Cache_type  = std::unordered_map<Int_type, Int_type>;

  template <bool partTwo>
  Vec_type ParseInput() const {
    static constexpr int numCopies = 5;
    Vec_type inputVec;
    for (const auto& str : input) {
      const auto v = Strings::Split(str, " ");

      std::string newV0 = v[0] + "?";
      std::string newV1 = v[1] + ",";
      if constexpr (partTwo) {
        for (int i = 0; i < numCopies - 1; ++i) {
          newV0 += v[0] + "?";
          newV1 += v[1] + ",";
        }
      }
      newV0.pop_back();
      newV1.pop_back();

      std::vector<int> intVec;
      for (const auto& vv : Strings::Split(newV1, ",")) {
        intVec.push_back(std::stoi(vv));
      }
      inputVec.emplace_back(newV0, intVec);
    }
    return inputVec;
  }

  Int_type Hash(Short_type pPattern, Short_type pGroup, Short_type currN) const {
    static constexpr auto shortSize = sizeof(Short_type) * 8;
    Int_type hash                   = 0;
    hash |= (static_cast<Int_type>(pPattern));
    hash |= (static_cast<Int_type>(pGroup) << (shortSize));
    hash |= (static_cast<Int_type>(currN) << (shortSize * 2));
    return hash;
  }

  Int_type Recurse(const std::string& pattern, const std::vector<int>& groups, Cache_type& dp,
                   Short_type currN = 0, Short_type pPattern = 0, Short_type pGroup = 0) const {
    const auto hash = Hash(pPattern, pGroup, currN);
    if (auto dpIt = dp.find(hash); dpIt != dp.end()) {
      return dpIt->second;
    }
    Int_type ans         = 0;
    const auto& patternC = pattern[pPattern];
    const auto groupN    = (pGroup < groups.size()) ? groups[pGroup] : 0;
    const bool newGroup  = (currN == groupN);
    if (pPattern == pattern.size()) {
      return ((pGroup + static_cast<int>(newGroup)) >= groups.size());
    }

    if (patternC == '#') {
      ++currN;
    } else if (patternC == '.') {
      if (currN != 0 && !newGroup) {
        return 0;
      }
      currN = 0;
    }

    // If we changed something previously that has now lead to too many consecutive #s.
    if (currN > groupN) {
      return 0;
    }

    if (patternC == '?') {
      // Change it to # if we can:
      if (currN + 1 <= groupN && !newGroup) {
        ans += Recurse(pattern, groups, dp, currN + 1, pPattern + 1, pGroup);
      }

      // Change it to . if we can:
      if (!currN || newGroup) {
        ans += Recurse(pattern, groups, dp, 0, pPattern + 1, pGroup + static_cast<int>(newGroup));
      }
    } else {
      ans += Recurse(pattern, groups, dp, currN, pPattern + 1, pGroup + static_cast<int>(newGroup));
    }
    dp[hash] = ans;
    return ans;
  }

  template <bool partTwo = false>
  Int_type GetArrangements() const {
    const Vec_type inputVec = ParseInput<partTwo>();
    Int_type ans            = 0;
    return std::accumulate(inputVec.begin(), inputVec.end(), Int_type{0},
                           [&](Int_type sm, const auto& v) {
                             Cache_type dp;
                             return sm + Recurse(v.first, v.second, dp);
                           });
  }

  std::string part_1() override { return std::to_string(GetArrangements()); }

  std::string part_2() override { return std::to_string(GetArrangements<true>()); }
};

};  // namespace AOC2023
