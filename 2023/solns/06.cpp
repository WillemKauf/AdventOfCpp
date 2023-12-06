/////////////////
//// std
/////////////////
#include <omp.h>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2023 {

struct day_06 : public Advent_type {
  static constexpr int year                 = 2023;
  static constexpr int date                 = 6;
  const std::vector<std::vector<int>> input = read_lines_vector_regex<int>(year, date, "(\\d+)");
  const std::vector<int>& times             = input[0];
  const std::vector<int>& distances         = input[1];

  using Int_type = int64_t;

  std::pair<Int_type, Int_type> GetBounds(Int_type time, Int_type maxDistance) const {
    const double a        = -1.0;
    const double b        = static_cast<double>(time);
    const double c        = -static_cast<double>(maxDistance);
    const double disc     = b * b - 4.0 * a * c;
    const double sqrtDisc = std::sqrt(disc);
    const auto xOne       = (-b + sqrtDisc) / (2.0 * a);
    const auto xTwo       = (-b - sqrtDisc) / (2.0 * a);
    const auto tOne       = std::floor(xOne) + 1;
    const auto tTwo       = std::ceil(xTwo) - 1;
    return {tOne, tTwo};
  }

  Int_type GetNumWays(Int_type time, Int_type maxDistance) const {
    const auto [s, e] = GetBounds(time, maxDistance);
    return e - s + 1;
  }

  std::string part_1() override {
    const auto& zipView = std::views::zip(times, distances);
    return std::to_string(
        std::accumulate(zipView.begin(), zipView.end(), 1, [&](int ans, const auto& zip) {
          return ans * GetNumWays(std::get<0>(zip), std::get<1>(zip));
        }));
  }

  std::string part_2() override {
    const auto toStringFunc = [](const auto& vec) {
      return std::stoll(std::accumulate(
          vec.begin(), vec.end(), std::string{},
          [](const std::string& s, const int& v) { return s + std::to_string(v); }));
    };

    const Int_type time     = toStringFunc(times);
    const Int_type distance = toStringFunc(distances);
    return std::to_string(GetNumWays(time, distance));
  }
};

};  // namespace AOC2023
