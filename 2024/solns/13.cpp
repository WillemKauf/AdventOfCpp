/////////////////
//// std
/////////////////
#include <cmath>
#include <complex>
#include <cstddef>
#include <cstdint>
#include <numeric>
#include <queue>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"
#include "algorithm.h"
#include "data_structure.h"
#include "grid.h"
#include "strings.h"

/////////////////
//// Eigen
/////////////////
#include <eigen3/Eigen/Dense>

namespace AOC2024 {

struct day_13 : public Advent_type {
  static constexpr int year = 2024;
  static constexpr int date = 13;

  const std::vector<std::vector<uint64_t>> input =
      read_lines_vector_regex<uint64_t>(year, date, "(\\d+)");

  struct Prize {
    uint64_t x;
    uint64_t y;
  };

  struct Button {
    uint64_t dx;
    uint64_t dy;
  };

  struct ClawMachine {
    Button a;
    Button b;
    Prize prize;
  };

  using Vec_type = std::vector<ClawMachine>;

  Vec_type ParseInput() const {
    Vec_type vec;
    for (const auto& v : std::views::chunk(input, 3)) {
      const auto& button_a = v[0];
      const auto& button_b = v[1];
      const auto& prize    = v[2];
      auto A               = Button{.dx = button_a[0], .dy = button_a[1]};
      auto B               = Button{.dx = button_b[0], .dy = button_b[1]};
      auto P               = Prize{.x = prize[0], .y = prize[1]};
      vec.emplace_back(A, B, P);
    }
    return vec;
  }

  const Vec_type machines = ParseInput();

  static uint64_t solve(const ClawMachine& machine) {
    /*
      x = A*(A_dx) + B*(B_dx)
      y = A*(a_dy) + B*(B_dy)
      i.e,
      [A_dx B_dx] [A] = [P_x]
      [A_dy B_dy] [B] = [P_y]
      Ax=b, duh.
     */
    Eigen::Matrix2d A;
    Eigen::Vector2d b;
    A << machine.a.dx, machine.b.dx, machine.a.dy, machine.b.dy;
    b << machine.prize.x, machine.prize.y;
    Eigen::Vector2d x = (A.transpose() * A).ldlt().solve(A.transpose() * b);
    const auto aa     = static_cast<uint64_t>(std::round(x[0]));
    const auto bb     = static_cast<uint64_t>(std::round(x[1]));
    const auto xx     = aa * machine.a.dx + bb * machine.b.dx;
    const auto yy     = aa * machine.a.dy + bb * machine.b.dy;
    if (xx == machine.prize.x && yy == machine.prize.y) {
      return 3 * aa + bb;
    }

    return 0;
  }

  uint64_t accumulate(const Vec_type& machine_vec) const {
    return std::accumulate(
        machine_vec.begin(), machine_vec.end(), uint64_t{0},
        [](uint64_t s, const ClawMachine& machine) { return s + solve(machine); });
  }

  std::string part_1() override { return std::to_string(accumulate(machines)); }

  std::string part_2() override {
    static constexpr auto TO_ADD = 10000000000000;
    auto machines_cpy            = machines;
    for (auto& machine : machines_cpy) {
      machine.prize.x += TO_ADD;
      machine.prize.y += TO_ADD;
    }

    return std::to_string(accumulate(machines_cpy));
  }
};

}  // namespace AOC2024
