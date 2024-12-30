#ifndef AOC_2024_H
#define AOC_2024_H

/////////////////
//// std
/////////////////
#include <memory>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "solns/01.cpp"
#include "solns/02.cpp"
#include "solns/03.cpp"
#include "solns/04.cpp"
#include "solns/05.cpp"
#include "solns/06.cpp"
#include "solns/07.cpp"
#include "solns/08.cpp"
#include "solns/09.cpp"
#include "solns/10.cpp"
#include "solns/11.cpp"
// #include "solns/12.cpp"
// #include "solns/13.cpp"
// #include "solns/14.cpp"
// #include "solns/15.cpp"
// #include "solns/16.cpp"
// #include "solns/17.cpp"
// #include "solns/18.cpp"
// #include "solns/19.cpp"
// #include "solns/20.cpp"
//  #include "solns/21.cpp"
//  #include "solns/22.cpp"
//  #include "solns/23.cpp"
//  #include "solns/24.cpp"
// #include "solns/25.cpp"

namespace AOC2024 {

struct AOC2024 : AdventYear_type {
  int year() override { return 2024; }

  AdventPtr_type MakeDay(int i) override {
    switch (i) {
      case 1:
        return std::make_unique<day_01>();
      case 2:
        return std::make_unique<day_02>();
      case 3:
        return std::make_unique<day_03>();
      case 4:
        return std::make_unique<day_04>();
      case 5:
        return std::make_unique<day_05>();
      case 6:
        return std::make_unique<day_06>();
      case 7:
        return std::make_unique<day_07>();
      case 8:
        return std::make_unique<day_08>();
      case 9:
        return std::make_unique<day_09>();
      case 10:
        return std::make_unique<day_10>();
      case 11:
        return std::make_unique<day_11>();
        //    case 12:
        //      return std::make_unique<day_12>();
        //    case 13:
        //      return std::make_unique<day_13>();
        //    case 14:
        //      return std::make_unique<day_14>();
        //    case 15:
        //      return std::make_unique<day_15>();
        //    case 16:
        //      return std::make_unique<day_16>();
        //    case 17:
        //      return std::make_unique<day_17>();
        //    case 18:
        //      return std::make_unique<day_18>();
        //    case 19:
        //      return std::make_unique<day_19>();
        //    case 20:
        //      return std::make_unique<day_20>();
        //    case 21:
        //      return std::make_unique<day_21>();
        //    case 22:
        //      return std::make_unique<day_22>();
        //    case 23:
        //      return std::make_unique<day_23>();
        //    case 24:
        //      return std::make_unique<day_24>();
        //   case 25:
        //       return std::make_unique<day_25>();
      default:
        return nullptr;
    }
  }

  void Run() override { RunImpl({10}); }
};

inline AdventYearPtr_type MakeYear() { return std::make_unique<AOC2024>(); }

inline void Run() { MakeYear()->Run(); }

};  // namespace AOC2024

#endif  // #ifndef AOC_2024_H
