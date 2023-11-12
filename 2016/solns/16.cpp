/////////////////
//// std
/////////////////
#include <ranges>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

namespace AOC2016 {

struct day_16 : public Advent_type {
  static constexpr int year = 2016;
  static constexpr int date = 16;
  const std::string input   = read_lines<std::string>(year, date)[0];

  std::string getChecksum(std::string_view data) {
    std::string checkSum;

    for (auto pair : std::views::chunk(data, 2)) {
      checkSum += (pair[0] == pair[1]) ? '1' : '0';
    }

    return (checkSum.size() % 2 == 0) ? getChecksum(checkSum) : checkSum;
  }

  std::string transformData(int diskSize, const std::string& startingData) {
    std::string data = startingData;

    while (data.size() < diskSize) {
      std::string b = data;
      std::reverse(b.begin(), b.end());
      std::for_each(b.begin(), b.end(), [](auto& c) { c = (c == '0') ? '1' : '0'; });
      data = data + '0' + b;
    }

    return getChecksum(data.substr(0, diskSize));
  }

  std::string part_1() override {
    static constexpr int diskSize = 272;
    return transformData(diskSize, input);
  }

  std::string part_2() override {
    static constexpr int diskSize = 35651584;
    return transformData(diskSize, input);
  }
};

};  // namespace AOC2016
