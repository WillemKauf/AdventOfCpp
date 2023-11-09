/////////////////
//// std
/////////////////
#include <algorithm>
#include <bitset>
#include <limits>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

namespace AOC2016 {

struct day_07 : public Advent_type {
  static constexpr int year            = 2016;
  static constexpr int date            = 7;
  const std::vector<std::string> input = read_lines<std::string>(year, date);

  std::string part_1() override {
    const auto isValid = [](std::string_view string) {
      std::string window     = "";
      bool inBrackets        = false;
      bool foundValid        = false;
      const auto validWindow = [](std::string_view window) {
        return ((window[0] == window[3]) && (window[1] == window[2]) && (window[0] != window[1]));
      };
      for (const auto& c : string) {
        if (c == '[' || c == ']') {
          inBrackets = (c == '[');
          window.clear();
          continue;
        } else {
          window += c;
          if (window.size() > 4) {
            window.erase(0, 1);
          }
          if (window.size() == 4) {
            if (validWindow(window)) {
              if (inBrackets) {
                return false;
              } else {
                foundValid = true;
              }
            }
          }
        }
      }
      return foundValid;
    };
    return std::to_string(std::count_if(input.begin(), input.end(), isValid));
  }

  std::string part_2() override {
    const auto isValid = [](std::string_view string) {
      std::string window                                       = "";
      bool inBrackets                                          = false;
      std::unordered_map<std::string, std::bitset<2>> foundBAB = {};
      const auto validWindow                                   = [](std::string_view window) {
        return ((window[0] == window[2]) && (window[0] != window[1]));
      };
      const auto oppositeBAB = [](const std::string& bab) -> std::string {
        return {bab[1], bab[0], bab[1]};
      };
      for (const auto& c : string) {
        if (c == '[' || c == ']') {
          inBrackets = (c == '[');
          window.clear();
          continue;
        } else {
          window += c;
          if (window.size() > 3) {
            window.erase(0, 1);
          }
          if (window.size() == 3) {
            if (validWindow(window)) {
              foundBAB[window].set(inBrackets);
              auto oppositeBABIt = foundBAB.find(oppositeBAB(window));
              if (oppositeBABIt != foundBAB.end() && (oppositeBABIt->second.test(!inBrackets))) {
                return true;
              }
            }
          }
        }
      }
      return false;
    };
    return std::to_string(std::count_if(input.begin(), input.end(), isValid));
  }
};

};  // namespace AOC2016
