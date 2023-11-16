/////////////////
//// std
/////////////////
#include <vector>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2016 {

struct day_21 : public Advent_type {
  static constexpr int year                         = 2016;
  static constexpr int date                         = 21;
  const std::vector<std::vector<std::string>> input = read_lines_vector_regex<std::string>(
      year, date, "(swap) (position) (\\d+) with position (\\d+)",
      "(swap) (letter) (\\w+) with letter (\\w+)", "(rotate) (\\w+) (\\d+) step",
      "(rotate) (based) on position of letter (\\w+)", "(reverse) positions (\\d+) through (\\d+)",
      "(move) position (\\d+) to position (\\d+)");

  static constexpr std::string scrambleStr = "abcdefgh";

  std::string PerformInstructs(std::string_view startingStr) {
    const int startingStrSize = startingStr.size();
    std::unordered_map<char, int> charMap;
    std::map<int, char> posMap;

    for (auto [index, c] : std::views::enumerate(startingStr)) {
      charMap[c]    = index;
      posMap[index] = c;
    }

    const auto rotateFunc = [&charMap, &posMap, startingStrSize](const auto rotAmount) {
      for (auto& [c, p] : charMap) {
        const int newP = p + rotAmount;
        p = Algorithm::Modulo(newP, startingStrSize);
        posMap.at(p)   = c;
      }
    };

    for (const auto& v : input) {
      if (v[0] == "swap") {
        if (v[1] == "position") {
          const auto a = std::stoi(v[2]);
          const auto b = std::stoi(v[3]);
          auto& cOne   = posMap.at(a);
          auto& cTwo   = posMap.at(b);
          std::swap(cOne, cTwo);
          std::swap(charMap.at(cOne), charMap.at(cTwo));
        } else if (v[1] == "letter") {
          const auto a = v[2][0];
          const auto b = v[3][0];
          auto& pOne   = charMap.at(a);
          auto& pTwo   = charMap.at(b);
          std::swap(pOne, pTwo);
          std::swap(posMap.at(pOne), posMap.at(pTwo));
        } else {
          throw std::runtime_error("Invalid swap v[1]");
        }
      } else if (v[0] == "rotate") {
        if (v[1] == "based") {
          const auto& basedC   = v[2][0];
          const auto& basedP   = charMap.at(basedC);
          const auto rotAmount = basedP + 1 + static_cast<int>(basedP >= 4);
          rotateFunc(rotAmount);
        } else {
          const auto mag       = std::stoi(v[2]);
          const auto rotAmount = (v[1] == "left") ? -mag : mag;
          rotateFunc(rotAmount);
        }
      } else if (v[0] == "reverse") {
        const auto x = std::stoi(v[1]);
        const auto y = std::stoi(v[2]);
        const auto d = y - x + 1;
        for (int i = 0; i < d / 2; ++i) {
          auto& cOne = posMap.at(x + i);
          auto& cTwo = posMap.at(y - i);
          std::swap(cOne, cTwo);
          std::swap(charMap.at(cOne), charMap.at(cTwo));
        }
      } else if (v[0] == "move") {
        const auto x    = std::stoi(v[1]);
        const auto y    = std::stoi(v[2]);
        auto& c         = posMap.at(x);
        auto newPosMap  = posMap;
        auto newCharMap = charMap;
        for (int i = x + 1; i <= y; ++i) {
          const auto& cc = posMap.at(i);
          auto& pp       = newCharMap.at(cc);
          --pp;
          newPosMap[pp] = cc;
        }
        for (int i = y; i < x; ++i) {
          const auto& cc = posMap.at(i);
          auto& pp       = newCharMap.at(cc);
          ++pp;
          newPosMap[pp] = cc;
        }
        newPosMap[y]  = c;
        newCharMap[c] = y;
        posMap        = std::move(newPosMap);
        charMap       = std::move(newCharMap);
      } else {
        throw std::runtime_error("Invalid v[0]");
      }
    }
    return std::accumulate(
        posMap.begin(), posMap.end(), std::string{},
        [](const auto& str, const auto& p) { return std::move(str) + p.second; });
  }

  std::string part_1() override { return PerformInstructs(scrambleStr); }

  std::string part_2() override {
    static constexpr std::string scrambledStr = "fbgdceah";
    for (const auto& scrambleStrPerm : Algorithm::GetPermutations(scrambleStr)) {
      if (PerformInstructs(scrambleStrPerm) == scrambledStr) {
        return scrambleStrPerm;
      }
    }

    std::unreachable();
  }
};

};  // namespace AOC2016
