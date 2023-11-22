/////////////////
//// std
/////////////////
#include <algorithm>
#include <numeric>
#include <unordered_map>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2017 {

struct day_04 : public Advent_type {
  static constexpr int year = 2017;
  static constexpr int date = 4;
  const std::vector<std::vector<std::string>> input =
      read_lines_vector_regex<std::string>(year, date, "(\\w+)");

  std::string part_1() override {
    return std::to_string(
        std::accumulate(input.begin(), input.end(), 0, [](int sm, const auto& words) {
          return sm + static_cast<int>(
                          std::unordered_set<std::string>(words.begin(), words.end()).size() ==
                          words.size());
        }));
  }

  std::string part_2() override {
    return std::to_string(
        std::accumulate(input.begin(), input.end(), 0, [](int sm, const auto& words) {
          bool hasAnagrams = false;
          for (auto [i, word] : std::views::enumerate(words)) {
            std::unordered_map<char, int> freqMp;
            for (const auto& c : word) {
              ++freqMp[c];
            }
            for (auto wordTwo : std::views::drop(words, i + 1)) {
              std::unordered_map<char, int> freqMpTwo;
              for (const auto& c : wordTwo) {
                ++freqMpTwo[c];
              }
              bool isAnagram = (freqMp.size() == freqMpTwo.size());
              if (isAnagram) {
                for (const auto& [c, f] : freqMp) {
                  if (freqMpTwo[c] != f) {
                    isAnagram = false;
                    break;
                  }
                }
              }
              hasAnagrams = isAnagram;
              if (hasAnagrams) {
                break;
              }
            }
            if (hasAnagrams) {
              break;
            }
          }
          return sm + static_cast<int>(!hasAnagrams);
        }));
  }
};

};  // namespace AOC2017
