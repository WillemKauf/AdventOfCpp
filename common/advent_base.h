#ifndef ADVENT_BASE_H
#define ADVENT_BASE_H

/////////////////
//// std
/////////////////
#include <array>
#include <functional>
#include <string>

using Return_type = std::pair<std::string, std::string>;

struct Advent_type {
  virtual std::string part_1() = 0;
  virtual std::string part_2() = 0;

  Return_type run_day() {
    const auto p_1 = part_1();
    const auto p_2 = part_2();
    return {p_1, p_2};
  }
};

#endif  // #ifndef ADVENT_BASE_H
