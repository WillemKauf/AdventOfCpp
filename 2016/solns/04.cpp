/////////////////
//// std
/////////////////
#include <unordered_map>
#include <utility>
#include <vector>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

namespace AOC2016 {

struct day_04 : public Advent_type {
  static constexpr int year            = 2016;
  static constexpr int date            = 4;
  const std::vector<std::string> input = read_lines<std::string>(year, date);

  static int validRoomId(std::string_view roomStr) {
    std::unordered_map<char, int> freqMp;
    std::string checkSum;
    std::string roomId;
    bool inCheckSum = false;
    int prevNum     = std::numeric_limits<int>::max();
    int index       = 0;
    std::vector<std::pair<char, int>> sortedVec;
    for (const auto& c : roomStr) {
      if (c == '[') {
        inCheckSum = true;
        sortedVec.insert(sortedVec.end(), freqMp.begin(), freqMp.end());
        std::sort(sortedVec.begin(), sortedVec.end(), [](const auto& pairA, const auto& pairB) {
          return std::tie(pairA.second, pairB.first) > std::tie(pairB.second, pairA.first);
        });
        continue;
      } else if (c == ']') {
        continue;
      } else if (std::isdigit(c)) {
        roomId += c;
      }

      if (inCheckSum) {
        if (sortedVec.at(index).first != c) {
          return 0;
        }
        ++index;
      } else {
        if (std::isalpha(c)) {
          ++freqMp[c];
        }
      }
    }
    return std::stoi(roomId);
  }

  std::string part_1() override {
    return std::to_string(std::accumulate(
        input.begin(), input.end(), 0,
        [](int sm, std::string_view roomStr) { return sm + validRoomId(roomStr); }));
  }

  std::string part_2() override {
    const auto northPoleStr = "northpole object storage";
    for (const auto& roomStr : input) {
      const auto roomStrSize  = roomStr.size();
      const auto checkSumSize = 7;
      const auto idSize       = 3;
      const auto startOfId    = roomStrSize - checkSumSize - idSize;
      const auto roomId       = std::stoi(roomStr.substr(startOfId, idSize));
      auto rotStr             = roomStr.substr(0, roomStr.find_last_of('-'));
      std::replace(rotStr.begin(), rotStr.end(), '-', ' ');
      for (int i = 0; i < roomId; ++i) {
        for (auto& c : rotStr) {
          if (!std::isspace(c)) {
            c += 1;
            if (c > 'z') {
              c = 'a';
            }
          }
        }
      }
      if (rotStr == northPoleStr) {
        return std::to_string(roomId);
      }
    }
    std::unreachable();
  }
};

};  // namespace AOC2016
