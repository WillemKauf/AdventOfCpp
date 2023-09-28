/////////////////
//// md5 lib
/////////////////
#include <openssl/md5.h>

/////////////////
//// std
/////////////////
#include <iomanip>
#include <unordered_set>
#include <utility>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

struct day_04 : public Advent_type {
  static constexpr int year = 2015;
  static constexpr int date = 4;
  const std::string input   = read_lines_string(year, date)[0];
  int startingIndex         = {};

  auto getIndex(int numZeros, int startIndex = 0) {
    const auto compareString = std::string(numZeros, '0');
    for (int i = startIndex; true; ++i) {
      unsigned char hashBuf[MD5_DIGEST_LENGTH];
      const auto toHash = input + std::to_string(i);
      MD5(reinterpret_cast<const unsigned char*>(toHash.c_str()), toHash.size(), hashBuf);
      std::ostringstream hashStream;
      hashStream << std::hex << std::setfill('0');
      for (const auto& c : hashBuf) {
        hashStream << std::setw(2) << static_cast<long long>(c);
      }
      if (hashStream.str().substr(0, numZeros) == compareString) {
        return std::make_pair(i, std::to_string(i));
      }
    }
    std::unreachable();
  }

  std::string part_1() override {
    auto result   = getIndex(5);
    startingIndex = result.first;
    return result.second;
  }

  std::string part_2() override { return getIndex(6, startingIndex).second; }
};
