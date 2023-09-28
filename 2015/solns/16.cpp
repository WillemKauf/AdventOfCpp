/////////////////
//// std
/////////////////
#include <limits>
#include <stdexcept>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

struct day_16 : public Advent_type {
  static constexpr int year = 2015;
  static constexpr int date = 16;

  using Map_type = std::unordered_map<std::string, int>;
  struct Sue {
    Sue(int num, Map_type map) : num(num), map(std::move(map)) {}
    int num;
    Map_type map;
  };

  std::vector<Sue> parseInput() const {
    const std::vector<std::vector<std::string>> rawInput = read_lines_vector_regex<std::string>(
        year, date, "Sue (\\d+): (\\w+): (\\d+), (\\w+): (\\d+), (\\w+): (\\d+)");
    std::vector<Sue> vec;
    vec.reserve(rawInput.size());
    for (const auto& v : rawInput) {
      const auto& sueNum        = std::stoi(v[0]);
      const auto& compoundOne   = v[1];
      const auto& numOne        = std::stoi(v[2]);
      const auto& compoundTwo   = v[3];
      const auto& numTwo        = std::stoi(v[4]);
      const auto& compoundThree = v[5];
      const auto& numThree      = std::stoi(v[6]);
      Map_type sueMap = {{compoundOne, numOne}, {compoundTwo, numTwo}, {compoundThree, numThree}};
      vec.emplace_back(sueNum, std::move(sueMap));
    }
    return vec;
  }

  const std::vector<Sue> input = parseInput();

  Map_type parseMap() const {
    Map_type mMap = {{"children", 3}, {"cats", 7},    {"samoyeds", 2}, {"pomeranians", 3},
                     {"akitas", 0},   {"vizslas", 0}, {"goldfish", 5}, {"trees", 3},
                     {"cars", 2},     {"perfumes", 1}};
    return mMap;
  }

  const Map_type matchMap = parseMap();

  std::string part_1() override {
    for (const auto& sue : input) {
      bool isMatch = true;
      for (const auto& [k, v] : sue.map) {
        if (matchMap.at(k) != v) {
          isMatch = false;
          break;
        }
      }
      if (isMatch) {
        return std::to_string(sue.num);
      }
    }
    std::unreachable();
  }

  std::string part_2() override {
    for (const auto& sue : input) {
      bool isMatch = true;
      for (const auto& [k, v] : sue.map) {
        const auto matchMapValue = matchMap.at(k);
        if (k == "cats" || k == "trees") {
          if (matchMapValue >= v) {
            isMatch = false;
            break;
          }
        } else if (k == "pomeranians" || k == "goldfish") {
          if (matchMapValue <= v) {
            isMatch = false;
            break;
          }
        } else {
          if (matchMapValue != v) {
            isMatch = false;
            break;
          }
        }
      }
      if (isMatch) {
        return std::to_string(sue.num);
      }
    }
    std::unreachable();
  }
};
