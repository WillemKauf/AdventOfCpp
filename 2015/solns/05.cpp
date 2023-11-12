/////////////////
//// std
/////////////////
#include <string>
#include <unordered_set>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

namespace AOC2015 {

struct day_05 : public Advent_type {
  static constexpr int year            = 2015;
  static constexpr int date            = 5;
  const std::vector<std::string> input = read_lines<std::string>(year, date);

  std::string part_1() override {
    const std::unordered_set<char> vowels = {'a', 'e', 'i', 'o', 'u'};
    const auto funcOne                    = [&vowels](std::string_view str) {
      return std::count_if(str.begin(), str.end(),
                                              [&vowels](char c) { return (vowels.contains(c)); }) >= 3;
    };

    const auto funcTwo = [](std::string_view str) {
      using Pair_type = std::pair<char, int>;
      return std::accumulate(std::next(str.begin()), str.end(), Pair_type{*str.begin(), 0},
                             [](const Pair_type& p, char c) {
                               return Pair_type{c, p.second + static_cast<int>(p.first == c)};
                             })
                 .second >= 1;
    };

    const std::unordered_set<std::string> badStrings = {"ab", "cd", "pq", "xy"};
    const auto funcThree                             = [&badStrings](std::string_view str) {
      return std::any_of(badStrings.begin(), badStrings.end(), [&str](std::string_view badString) {
               return str.find(badString) != std::string::npos;
             }) == 0;
    };

    const std::vector<std::function<bool(std::string_view)>> lambdaFuncs = {funcOne, funcTwo,
                                                                            funcThree};

    return std::to_string(
        std::count_if(input.begin(), input.end(), [&lambdaFuncs](std::string_view str) {
          return std::all_of(lambdaFuncs.begin(), lambdaFuncs.end(),
                             [&str](const auto& func) { return func(str); });
        }));
  }

  std::string part_2() override {
    const auto funcOne = [](std::string_view str) {
      std::unordered_map<std::string, int> indexMp;
      using Pair_type = std::pair<char, int>;
      return (std::accumulate(std::next(str.begin()), str.end(), Pair_type{*str.begin(), 0},
                              [&indexMp, idx = 1](const Pair_type& p, char c) mutable {
                                const auto pairStr   = std::string{p.first} + c;
                                auto& lastIndex      = indexMp[pairStr];
                                lastIndex            = (lastIndex == 0) ? idx : lastIndex;
                                const auto indexDiff = idx - lastIndex;
                                ++idx;
                                return Pair_type{c, p.second + static_cast<int>(indexDiff > 1)};
                              }))
                 .second >= 1;
    };

    const auto funcTwo = [](std::string_view str) {
      using Tuple_type = std::tuple<char, char, int>;
      return std::get<2>(std::accumulate(
                 std::next(str.begin(), 2), str.end(),
                 Tuple_type{*str.begin(), *std::next(str.begin()), 0},
                 [](const Tuple_type& p, char c) {
                   const auto& [charOne, charTwo, currValue] = p;
                   return Tuple_type{charTwo, c, currValue + static_cast<int>(c == charOne)};
                 })) >= 1;
    };

    const std::vector<std::function<bool(std::string_view)>> lambdaFuncs = {funcOne, funcTwo};

    return std::to_string(
        std::count_if(input.begin(), input.end(), [&lambdaFuncs](std::string_view str) {
          return std::all_of(lambdaFuncs.begin(), lambdaFuncs.end(),
                             [&str](const auto& func) { return func(str); });
        }));
  }
};

};  // namespace AOC2015
