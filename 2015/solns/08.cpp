/////////////////
//// std
/////////////////
#include <algorithm>
#include <numeric>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

struct day_08 : public Advent_type {
  static constexpr int year = 2015;
  static constexpr int date = 8;

  const std::vector<std::string> input = read_lines_string(year, date);

  int getNumberChars(std::string_view stringView) {
    int numChars = 0;
    int i        = 0;
    const auto n = stringView.size();
    while (i < n) {
      const auto& c = stringView[i];
      if (c == '\\') {
        if (i < n - 1) {
          const auto& cc = stringView[i + 1];
          if (cc == '"') {
            ++numChars;
            i += 2;
          } else if (cc == '\\') {
            ++numChars;
            i += 2;
          } else if (cc == 'x') {
            ++numChars;
            i += 4;
          }
        }
      } else if (c == '"') {
        ++i;
      } else {
        ++numChars;
        ++i;
      }
    }
    return numChars;
  }

  std::string part_1() override {
    return std::to_string(std::accumulate(
        input.begin(), input.end(), 0,
        [&](int sm, std::string_view stringView) { return sm + stringView.size() - getNumberChars(stringView); }));
  }

  std::string encodeString(std::string_view stringView) {
    std::string newString = "\"";
    for(const auto& c : stringView){
      if(c == '"'){
        newString += "\\\"";
      }else if(c == '\\'){
        newString += "\\\\";
      }else{
        newString += c;
      }
    }
    newString += "\"";
    return newString;
  }

  std::string part_2() override {
    return std::to_string(std::accumulate(
        input.begin(), input.end(), 0,
        [&](int sm, std::string_view stringView) { return sm + encodeString(stringView).size() - stringView.size(); }));
  }
};
