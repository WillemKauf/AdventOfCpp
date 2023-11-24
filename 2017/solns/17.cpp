/////////////////
//// std
/////////////////
#include <deque>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2017 {

struct day_17 : public Advent_type {
  static constexpr int year = 2017;
  static constexpr int date = 17;
  const int input           = read_lines<int>(year, date)[0];

  std::string part_1() override {
    static constexpr int numOperations = year;
    std::deque<int> dq;
    for (int i = 0; i <= numOperations; ++i) {
      Algorithm::RotateDeque(dq, -(input + 1));
      dq.push_front(i);
    }
    dq.pop_front();
    return std::to_string(dq.front());
  }

  std::string part_2() override {
    static constexpr int numOperations = 5e7;
    // Deque approach. Very slow!
    // std::deque<int> dq;
    // int valueAfterZero = {};
    // for (int i = 0; i <= numOperations; ++i) {
    //   Algorithm::RotateDeque(dq, -input);
    //   if (dq.front() == 0) {
    //     valueAfterZero = i;
    //   }
    //   Algorithm::RotateDeque(dq, -1);
    //   dq.push_front(i);
    // }

    // Simply track value of position after zero.
    int p              = 0;
    int valueAfterZero = {};
    for (int i = 1; i <= numOperations; ++i) {
      p = (p + input) % i + 1;
      if (p == 1) {
        valueAfterZero = i;
      }
    }
    return std::to_string(valueAfterZero);
  }
};

};  // namespace AOC2017
