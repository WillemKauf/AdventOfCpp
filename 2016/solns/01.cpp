/////////////////
//// std
/////////////////
#include <complex>
#include <functional>
#include <unordered_map>
#include <utility>
#include <vector>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

namespace AOC2016 {

struct day_01 : public Advent_type {
  static constexpr int year = 2016;
  static constexpr int date = 1;
  const std::vector<std::string> input =
      read_lines_vector_regex<std::string>(year, date, "\\s?([^,]+)")[0];

  using Complex_type = std::complex<int>;

  std::string part_1() override {
    Complex_type position(0, 0);
    Complex_type dir(0, 1);
    for (const auto& v : input) {
      const char& dd = v[0];
      const int& mag = std::stoi(v.substr(1));
      switch (dd) {
        case 'L':
          dir *= std::complex(0, 1);
          break;
        case 'R':
          dir *= std::complex(0, -1);
          break;
      }
      position += mag * dir;
    }
    return std::to_string(std::abs(position.real()) + std::abs(position.imag()));
  }

  std::string part_2() override {
    Complex_type position(0, 0);
    Complex_type dir(0, 1);
    std::unordered_set<int> seen;
    const auto hsh = [](const auto& complexNum) {
      return (7919 + std::hash<int>{}(complexNum.real())) * 7919 +
             std::hash<int>{}(complexNum.imag());
    };
    for (const auto& v : input) {
      const char& dd = v[0];
      const int& mag = std::stoi(v.substr(1));
      switch (dd) {
        case 'L':
          dir *= std::complex(0, 1);
          break;
        case 'R':
          dir *= std::complex(0, -1);
          break;
      }
      Complex_type newPosition = position;
      for (int i = 0; i < mag; ++i) {
        newPosition += dir;
        int positionHash = hsh(newPosition);
        if (seen.contains(positionHash)) {
          return std::to_string(std::abs(newPosition.real()) + std::abs(newPosition.imag()));
        }
        seen.insert(positionHash);
      }
      position = newPosition;
    }
    std::unreachable();
  }
};

};  // namespace AOC2016
