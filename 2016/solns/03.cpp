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

struct day_03 : public Advent_type {
  static constexpr int year                 = 2016;
  static constexpr int date                 = 3;
  const std::vector<std::vector<int>> input = read_lines_vector_regex<int>(year, date, "(\\d+)");

  static bool isValidTriangle(const std::vector<int>& vec) {
    const auto sm = std::accumulate(vec.begin(), vec.end(), 0);
    const auto mx = *std::max_element(vec.begin(), vec.end());
    return (sm - mx) > mx;
  }

  std::string part_1() override {
    return std::to_string(std::count_if(input.begin(), input.end(), isValidTriangle));
  }

  std::string part_2() override {
    int n = 0;
    for (const auto& tripleVec : std::views::chunk(input, 3)) {
      const std::vector<int> triangleOne   = {tripleVec[0][0], tripleVec[1][0], tripleVec[2][0]};
      const std::vector<int> triangleTwo   = {tripleVec[0][1], tripleVec[1][1], tripleVec[2][1]};
      const std::vector<int> triangleThree = {tripleVec[0][2], tripleVec[1][2], tripleVec[2][2]};

      n += isValidTriangle(triangleOne) + isValidTriangle(triangleTwo) +
           isValidTriangle(triangleThree);
    }
    return std::to_string(n);
  }
};

};  // namespace AOC2016
