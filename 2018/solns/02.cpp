/////////////////
//// std
/////////////////

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2018 {

struct day_02 : public Advent_type {
  static constexpr int year            = 2018;
  static constexpr int date            = 2;
  const std::vector<std::string> input = read_lines<std::string>(year, date);

  std::string part_1() override {
    using Pair_type = std::pair<int, int>;
    const auto [twoLetter, threeLetter] =
        std::accumulate(input.begin(), input.end(), Pair_type{},
                        [](const auto& pair, const auto& word) -> Pair_type {
                          const auto counter = DataStructure::Counter(word);
                          bool isTwo         = false;
                          bool isThree       = false;
                          for (const auto& [k, v] : counter) {
                            isTwo |= (v == 2);
                            isThree |= (v == 3);
                          }
                          return {pair.first + isTwo, pair.second + isThree};
                        });
    return std::to_string(twoLetter * threeLetter);
  }

  std::string part_2() override {
    for (auto [i, wordOne] : std::views::enumerate(input)) {
      for (auto wordTwo : std::views::drop(input, i + 1)) {
        std::unordered_set<int> diffChars;
        for (auto [i, chars] : std::views::zip(wordOne, wordTwo) | std::views::enumerate) {
          const auto& charOne = std::get<0>(chars);
          const auto& charTwo = std::get<1>(chars);
          if (charOne != charTwo) {
            diffChars.insert(i);
          }
        }
        if (diffChars.size() == 1) {
          auto returnWord = wordOne;
          for (const auto& diffChar : diffChars) {
            returnWord.erase(diffChar, 1);
          }
          return returnWord;
        }
      }
    }
    std::unreachable();
  }
};

};  // namespace AOC2018
