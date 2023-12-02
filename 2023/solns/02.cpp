/////////////////
//// std
/////////////////

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2023 {

struct day_02 : public Advent_type {
  static constexpr int year            = 2023;
  static constexpr int date            = 2;
  const std::vector<std::string> input = read_lines<std::string>(year, date);

  using Map_type   = std::unordered_map<std::string, int>;
  using Round_type = std::vector<Map_type>;
  using Input_type = std::vector<std::pair<int, Round_type>>;

  Input_type ParseInput() const {
    Input_type gameVec;
    gameVec.reserve(input.size());
    for (const auto& [index, game] : std::views::enumerate(input)) {
      Round_type rounds;
      for (const auto& round : Strings::Split(game.substr(game.find_first_of(':') + 2), "; ")) {
        Map_type bag;
        const auto cubesSet = Strings::Split(round, ", ");
        for (const auto& cubeStr : cubesSet) {
          const auto spaceP    = cubeStr.find_first_of(' ');
          const auto numCubes  = std::stoi(cubeStr.substr(0, spaceP));
          const auto cubeColor = cubeStr.substr(spaceP + 1);
          bag[cubeColor]       = numCubes;
        }
        rounds.push_back(std::move(bag));
      }
      gameVec.emplace_back(index + 1, std::move(rounds));
    }
    return gameVec;
  }

  const Input_type gameVec = ParseInput();

  std::string part_1() override {
    static constexpr auto numRed                             = 12;
    static constexpr auto numGreen                           = 13;
    static constexpr auto numBlue                            = 14;
    static const std::unordered_map<std::string, int> maxMap = {
        {"red", numRed}, {"green", numGreen}, {"blue", numBlue}};

    return std::to_string(
        std::accumulate(gameVec.begin(), gameVec.end(), 0, [&](int sm, const auto& pair) {
          const auto& index = pair.first;
          const auto& v     = pair.second;
          bool isValid      = true;
          for (const auto& round : v) {
            for (const auto& [c, n] : round) {
              if (auto maxIt = maxMap.find(c); maxIt != maxMap.end()) {
                if (n > maxIt->second) {
                  isValid = false;
                  break;
                }
              }
              if (!isValid) {
                break;
              }
            }
            if (!isValid) {
              break;
            }
          }
          return sm + ((isValid) ? index : 0);
        }));
  }

  std::string part_2() override {
    return std::to_string(
        std::accumulate(gameVec.begin(), gameVec.end(), 0, [&](int sm, const auto& pair) {
          const auto& index                               = pair.first;
          const auto& v                                   = pair.second;
          std::unordered_map<std::string, int> currMinMap = {};
          for (const auto& round : v) {
            for (const auto& [c, n] : round) {
              currMinMap[c] = std::max(currMinMap[c], n);
            }
          }
          const auto res = std::accumulate(currMinMap.begin(), currMinMap.end(), 1,
                                           [](int mul, const auto& p) { return mul * p.second; });
          return sm + res;
        }));
  }
};

};  // namespace AOC2023
