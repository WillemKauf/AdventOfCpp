/////////////////
//// std
/////////////////
#include <deque>
#include <list>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2016 {

struct day_19 : public Advent_type {
  static constexpr int year = 2016;
  static constexpr int date = 19;
  const int input           = std::stoi(read_lines<std::string>(year, date)[0]);

  std::string part_1() override {
    // This solution is suboptimal as far as data layout goes due to std::list.
    // It's here for completion's sake, and because it's still a fun solution to learn from.
    // std::list<std::pair<int, int>> elves;
    // for (int i = 1; i <= input; ++i) {
    //   elves.emplace_back(1, i);
    // }
    //
    // auto p = elves.begin();
    //
    // while (elves.size() > 1) {
    //   auto pNext = std::next(p);
    //
    //   if (pNext == elves.end()) {
    //     pNext = elves.begin();
    //   }
    //
    //   p->first += pNext->first;
    //
    //   pNext = elves.erase(pNext);
    //
    //   if (pNext == elves.end()) {
    //     pNext = elves.begin();
    //   }
    //
    //   p = pNext;
    // }

    std::deque<int> elves(input);
    std::iota(elves.begin(), elves.end(), 1);
    while (elves.size() > 1) {
      // WOW the STL rotate is slow. Why isn't there an std::deque.rotate() specialization?
      // std::rotate(elves.begin(), elves.begin()+1, elves.end());
      Algorithm::RotateDeque(elves, -1);
      elves.pop_front();
    }

    return std::to_string(elves.front());
  }

  std::string part_2() override {
    // B = (A + n/2)%n
    std::deque<int> elves(input);
    std::iota(elves.begin(), elves.end(), 1);
    int offset = 0;
    while (elves.size() > 1) {
      const int rotate = elves.size() / 2;
      // WOW the STL rotate is slow. Why isn't there an std::deque.rotate() specialization?
      // std::rotate(elves.begin(), elves.begin()+1, elves.end());
      Algorithm::RotateDeque(elves, -(rotate - offset));
      elves.pop_front();
      offset = (rotate - 1) % elves.size();
    }

    return std::to_string(elves.front());
  }
};

};  // namespace AOC2016
