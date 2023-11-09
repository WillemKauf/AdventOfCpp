/////////////////
//// std
/////////////////
#include <algorithm>
#include <cstdint>
#include <limits>
#include <numeric>
#include <queue>
#include <stdexcept>
#include <unordered_map>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

namespace AOC2015 {

struct day_11 : public Advent_type {
  static constexpr int year = 2015;
  static constexpr int date = 11;

  const std::string input     = read_lines_string(year, date)[0];
  std::string initialPassword = {};

  void recursiveIncrement(std::string& password, std::size_t index) {
    auto& c = password[index];
    if (c == 'z') {
      c = 'a';
      recursiveIncrement(password, index - 1);
    } else {
      ++c;
    }
  }

  void incrementPassword(std::string& password) {
    std::string s = "a";
    recursiveIncrement(password, password.size() - 1);
  }

  std::string findPassword(const std::string& startingPassword) {
    const auto funcOne = [](std::string_view str) {
      using Tuple_type = std::tuple<char, char, int>;
      return std::get<2>(std::accumulate(
                 std::next(str.begin(), 2), str.end(),
                 Tuple_type{*str.begin(), *std::next(str.begin()), 0},
                 [](const Tuple_type& p, char c) {
                   const auto& [charOne, charTwo, currValue] = p;
                   const auto diffOne                        = charTwo - charOne;
                   const auto diffTwo                        = c - charTwo;
                   const auto isStraight                     = (diffOne == 1) && (diffTwo == 1);
                   return Tuple_type{charTwo, c, currValue + static_cast<int>(isStraight)};
                 })) >= 1;
    };

    const std::unordered_set<char> badChars = {'i', 'o', 'l'};
    const auto funcTwo                      = [&badChars](std::string_view str) {
      return std::any_of(badChars.begin(), badChars.end(), [&str](char badChar) {
               return str.find(badChar) != std::string::npos;
             }) == 0;
    };

    const auto funcThree = [](std::string_view str) {
      std::unordered_set<char> seenPairs;
      using Pair_type = std::pair<char, int>;
      return (std::accumulate(std::next(str.begin()), str.end(), Pair_type{*str.begin(), 0},
                              [&seenPairs](const Pair_type& p, char c) mutable {
                                const auto isNewPair = (p.first == c) && !seenPairs.contains(c);
                                if (isNewPair) {
                                  seenPairs.insert(c);
                                }
                                return Pair_type{c, p.second + static_cast<int>(isNewPair)};
                              }))
                 .second > 1;
    };

    const std::vector<std::function<bool(std::string_view)>> lambdaFuncs = {funcOne, funcTwo,
                                                                            funcThree};
    const auto isValidPassword = [&lambdaFuncs](std::string_view passwordStr) {
      return std::all_of(lambdaFuncs.begin(), lambdaFuncs.end(),
                         [&passwordStr](const auto& func) { return func(passwordStr); });
    };

    std::string password = startingPassword;
    while (!isValidPassword(password)) {
      incrementPassword(password);
    }
    initialPassword = password;
    return password;
  }

  std::string part_1() override { return findPassword(input); }

  std::string part_2() override {
    incrementPassword(initialPassword);
    return findPassword(initialPassword);
  }
};

};  // namespace AOC2015
