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
#include "data_structure.h"
#include "grid.h"
#include "strings.h"

namespace {

enum class Op {
  ADD,
  MUL,
  CAT
};

template <bool part_two>
struct Ops {};

template <>
struct Ops<false> {
  static constexpr auto ops = std::to_array({Op::ADD, Op::MUL});
};

template <>
struct Ops<true> {
  static constexpr auto ops = std::to_array({Op::ADD, Op::MUL, Op::CAT});
};

}  // namespace

namespace AOC2024 {

struct day_07 : public Advent_type {
  static constexpr int year = 2024;
  static constexpr int date = 7;

  using Scalar_type = uint64_t;
  const std::vector<std::vector<Scalar_type>> input =
      read_lines_vector_regex<Scalar_type>(year, date, "(\\d+)");

  static Scalar_type do_op(Scalar_type a, Scalar_type b, Op op) {
    switch (op) {
      case Op::ADD:
        return a + b;
      case Op::MUL:
        return a * b;
      case Op::CAT: {
        return a * std::pow(10, static_cast<Scalar_type>(std::log10(b)) + 1) + b;
      }
      default:
        throw std::runtime_error("");
    }
  }

  template <bool part_two>
  static bool equation_is_true(const std::vector<Scalar_type>& eq, Scalar_type curr,
                               size_t index = 2) {
    const auto& goal = eq[0];
    if (curr > goal) {
      return false;
    }

    if (index == eq.size()) {
      return curr == goal;
    }

    for (const Op op : Ops<part_two>::ops) {
      if (equation_is_true<part_two>(eq, do_op(curr, eq[index], op), index + 1)) {
        return true;
      }
    }

    return false;
  }

  template <bool part_two>
  Scalar_type accumulate_true_equations() const {
    return std::accumulate(input.begin(), input.end(), Scalar_type{0},
                           [](Scalar_type s, const auto& eq) {
                             if (equation_is_true<part_two>(eq, eq[1])) {
                               s += eq[0];
                             }
                             return s;
                           });
  }

  std::string part_1() override { return std::to_string(accumulate_true_equations<false>()); }

  std::string part_2() override { return std::to_string(accumulate_true_equations<true>()); }
};

}  // namespace AOC2024
