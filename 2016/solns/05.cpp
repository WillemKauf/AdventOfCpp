/////////////////
//// std
/////////////////
#include <bitset>
#include <unordered_map>
#include <utility>
#include <vector>

/////////////////
//// local
/////////////////
#include "../common/common.h"
#include "../common/advent_base.h"
#include "../common/read_input.h"

namespace AOC2016 {

struct day_05 : public Advent_type {
  static constexpr int year         = 2016;
  static constexpr int date         = 5;
  const std::string input           = read_lines<std::string>(year, date)[0];
  static constexpr int passwordSize = 8;
  std::string passwordPartOne       = {};
  std::string passwordPartTwo       = std::string(passwordSize, '\0');

  std::string part_1() override {
    const auto numZeros      = 5;
    const auto compareString = std::string(numZeros, '0');
    int passwordPartTwoSize  = 0;
    std::bitset<passwordSize> setBits;
    for (int i = 0; passwordPartTwoSize != passwordSize; ++i) {
      const auto toHash = input + std::to_string(i);
      const auto hashStr = Hash::GetMD5Hash(toHash);
      if (hashStr.substr(0, numZeros) == compareString) {
        if (passwordPartOne.size() < passwordSize) {
          passwordPartOne += hashStr[numZeros];
        }
        const auto pos = hashStr[numZeros] - '0';
        if (pos < passwordSize && !(setBits[pos])) {
          const auto chr = hashStr[numZeros + 1];
          setBits.set(pos);
          passwordPartTwo[pos] = chr;
          ++passwordPartTwoSize;
        }
      }
    }
    return passwordPartOne;
  }

  std::string part_2() override { return passwordPartTwo; }
};

};  // namespace AOC2016
