/////////////////
//// std
/////////////////
#include <cstdint>
#include <limits>
#include <map>
#include <unordered_map>

/////////////////
//// openMP
/////////////////
#include <omp.h>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2023 {

struct day_05 : public Advent_type {
  static constexpr int year = 2023;
  static constexpr int date = 5;
  const std::vector<std::vector<std::string>> input =
      read_lines_vector_regex<std::string>(year, date, "(\\d+)", "(\\w+)-to-(\\w+) (map):");

  using Int_type         = uint32_t;
  using IntervalMap_type = std::map<Int_type, Int_type>;

  struct Value {
    std::string destName;
    IntervalMap_type intervalMap;
  };

  using Seeds_type    = std::vector<Int_type>;
  using ValueMap_type = std::unordered_map<std::string, Value>;

  struct Input {
    std::vector<Int_type> seeds;
    ValueMap_type valueMap;
  };

  Input ParseInput() const {
    std::vector<Int_type> seeds;
    const auto& seedStrs = input[0];
    seeds.reserve(seedStrs.size());

    for (const auto& seedStr : seedStrs) {
      seeds.push_back(std::stol(seedStr));
    }

    std::string currKey;
    Value currValue;
    ValueMap_type valueMap;
    for (const auto& v : std::views::drop(input, 1)) {
      if (v.back() == "map") {
        if (!currKey.empty()) {
          valueMap.insert_or_assign(currKey, currValue);
        }
        currValue = Value{v[1]};
        currKey   = v[0];
      } else if (v.size() != 0) {
        const Int_type destStart   = std::stol(v[0]);
        const Int_type sourceStart = std::stol(v[1]);
        const Int_type rangeLength = std::stol(v[2]);
        const Int_type sourceEnd   = sourceStart + rangeLength;
        auto& intervalMap          = currValue.intervalMap;
        intervalMap[sourceStart]   = destStart;
        if (!intervalMap.contains(sourceEnd)) {
          intervalMap[sourceEnd] = sourceEnd;
        }
      }
    }

    if (!currKey.empty()) {
      valueMap.insert_or_assign(currKey, currValue);
    }

    return {seeds, valueMap};
  }

  const Input inputContainers   = ParseInput();
  const Seeds_type& seeds       = inputContainers.seeds;
  const ValueMap_type& valueMap = inputContainers.valueMap;

  Int_type Recurse(Int_type source, std::string key = "seed") {
    if (key == "location") {
      return source;
    }

    const auto& map         = valueMap.at(key);
    const auto& intervalMap = map.intervalMap;
    Int_type dest           = source;
    if (auto intervalMapIt = intervalMap.lower_bound(source); intervalMapIt != intervalMap.end()) {
      const auto minBoundIt =
          (intervalMapIt->first != source) ? std::prev(intervalMapIt) : intervalMapIt;
      const Int_type dist = source - minBoundIt->first;
      dest                = minBoundIt->second + dist;
    }

    return Recurse(dest, map.destName);
  }

  std::string part_1() override {
    return std::to_string(
        std::accumulate(seeds.begin(), seeds.end(), std::numeric_limits<Int_type>::max(),
                        [&](Int_type ans, Int_type seed) { return std::min(ans, Recurse(seed)); }));
  }

  std::string part_2() override {
    Int_type ans = std::numeric_limits<Int_type>::max();
    for (const auto& seedRange : std::views::chunk(seeds, 2)) {
      const Int_type seedMin = seedRange[0];
      const Int_type seedMax = seedMin + seedRange[1];
#pragma omp parallel for reduction(min : ans)
      for (Int_type i = seedMin; i < seedMax; ++i) {
        ans = std::min(ans, Recurse(i));
      }
    }
    return std::to_string(ans);
  }
};

};  // namespace AOC2023
