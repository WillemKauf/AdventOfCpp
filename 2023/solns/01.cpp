/////////////////
//// std
/////////////////
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2023 {

struct day_01 : public Advent_type {
  static constexpr int year            = 2023;
  static constexpr int date            = 1;
  const std::vector<std::string> input = read_lines<std::string>(year, date);

  static bool FindFunc(char c) { return std::isdigit(c); }

  std::string part_1() override {
    return std::to_string(std::accumulate(input.begin(), input.end(), 0, [](int n, const auto& s) {
      const int firstN = *std::find_if(s.begin(), s.end(), FindFunc) - '0';
      const int lastN  = *std::find_if(s.rbegin(), s.rend(), FindFunc) - '0';
      return n + firstN * 10 + lastN;
    }));
  }

  std::string part_2() override {
    const std::unordered_map<std::string, char> validDigitsMap = {
        {"one", '1'}, {"two", '2'},   {"three", '3'}, {"four", '4'}, {"five", '5'},
        {"six", '6'}, {"seven", '7'}, {"eight", '8'}, {"nine", '9'}};

    auto ReplaceLeft = [&](auto& s) {
      int minP = s.size();
      char c   = {};
      for (const auto& [k, v] : validDigitsMap) {
        const auto p = s.find(k);
        if (p < minP) {
          c    = v;
          minP = p;
        }
      }
      if (minP != s.size()) {
        s[minP + 1] = c;
      }
    };

    auto ReplaceRight = [&](auto& s) {
      int maxP = -1;
      char c   = {};
      for (const auto& [k, v] : validDigitsMap) {
        const auto p = s.rfind(k);
        if (p != std::string::npos && p > maxP) {
          c    = v;
          maxP = p;
        }
      }
      if (maxP != -1) {
        s[maxP + 1] = c;
      }
    };

    auto strings = input;
    return std::to_string(std::accumulate(strings.begin(), strings.end(), 0, [&](int n, auto& s) {
      ReplaceLeft(s);
      ReplaceRight(s);
      const int firstN = *std::find_if(s.begin(), s.end(), FindFunc) - '0';
      const int lastN  = *std::find_if(s.rbegin(), s.rend(), FindFunc) - '0';
      return n + firstN * 10 + lastN;
    }));
  }
};

};  // namespace AOC2023
