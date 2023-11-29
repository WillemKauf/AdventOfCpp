#ifndef ADVENT_BASE_H
#define ADVENT_BASE_H

/////////////////
//// std
/////////////////
#include <array>
#include <chrono>
#include <format>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>

static constexpr std::string DayTwentyFiveString = "Day 25!";

using Return_type = std::pair<std::string, std::string>;

struct Advent_type {
  virtual std::string part_1() = 0;
  virtual std::string part_2() = 0;

  virtual ~Advent_type() = default;

  Return_type run_day() {
    const auto p_1 = part_1();
    const auto p_2 = part_2();
    return {p_1, p_2};
  }
};

using AdventPtr_type = std::unique_ptr<Advent_type>;

inline auto TimeDay(int i, AdventPtr_type day) {
  const auto start = std::chrono::system_clock::now();
  const auto res   = day->run_day();
  const auto end   = std::chrono::system_clock::now();
  const auto dt    = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
  std::cout << std::format("Day #{:<2}: Part 1: {:<10}, Part 2: {:<10} | {} μs\n", i, res.first,
                           res.second, dt);
  return dt;
}

struct AdventYear_type {
  virtual int year()                    = 0;
  virtual AdventPtr_type MakeDay(int i) = 0;
  virtual void Run() {
    std::vector<int> dates(25);
    std::iota(dates.begin(), dates.end(), 1);
    RunImpl(dates);
  }

  virtual ~AdventYear_type() = default;

  void RunImpl(const std::vector<int>& dates) {
    std::cout << std::format("================{}================\n", year());
    double t                                = 0.0;
    static constexpr auto secondsConversion = 1.0e-6;
    for (const auto& i : dates) {
      auto day = MakeDay(i);
      t += TimeDay(i, std::move(day));
    }
    std::cout << "-------------------------------------\n";
    std::cout << "Total time: " << t * secondsConversion << "s\n";
  }
};

using AdventYearPtr_type = std::unique_ptr<AdventYear_type>;

#endif  // #ifndef ADVENT_BASE_H
