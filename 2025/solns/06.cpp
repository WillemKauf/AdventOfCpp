#include <cstddef>
#include <cstdint>
#include <numeric>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"
#include "algorithm.h"
#include "data_structure.h"
#include "grid.h"

namespace AOC2025 {

struct day_06 : public Advent_type {
  static constexpr int year = 2025;
  static constexpr int date = 6;
  using Vec_type            = std::vector<std::vector<std::string>>;
  const Vec_type input_vec  = read_lines_vector_regex<std::string>(year, date, R"(\d+|[+*])");

  enum class Op {
    ADD,
    MUL
  };

  struct Problem {
    Op op;
    std::vector<uint64_t> vals;
  };

  using Input = std::vector<Problem>;

  Input ParseInput() {
    Input input;
    auto num_ops = input_vec[0].size();
    input.resize(num_ops);
    for (const auto& vec : input_vec) {
      for (auto [i, v] : std::views::enumerate(vec)) {
        if (v[0] == '+' || v[0] == '*') {
          input[i].op = v[0] == '+' ? Op::ADD : Op::MUL;
        } else {
          input[i].vals.push_back(std::stoull(v));
        }
      }
    }
    return input;
  }

  const Input input = ParseInput();

  std::string part_1() override {
    uint64_t res = 0;
    for (const auto& p : input) {
      auto func = [&](auto a, auto b) {
        if (p.op == Op::ADD) {
          return a + b;
        } else {
          return a * b;
        }
      };

      uint64_t init = p.op == Op::ADD ? 0 : 1;
      res += std::accumulate(p.vals.begin(), p.vals.end(), init, func);
    }
    return std::to_string(res);
  }

  Input parse_shifted_input() {
    Input shifted_input;
    const std::vector<std::string> input_vec = read_lines<std::string>(year, date);
    std::vector<uint64_t> ns;
    int i  = 0;
    bool b = true;
    Problem p;
    while (b) {
      uint64_t n     = 0;
      b              = false;
      bool all_empty = true;
      for (int j = 0; j < input_vec.size(); ++j) {
        auto& row = input_vec[j];
        if (i < row.size()) {
          b       = true;
          auto& c = row[i];
          if (c == ' ') {
            continue;
          }
          all_empty = false;
          if (std::isdigit(c)) {
            n = n * 10 + c - '0';
          }
        }
      }

      if (all_empty) {
        p.op = input[shifted_input.size()].op;
        shifted_input.push_back(std::move(p));
        p = Problem{};
      } else {
        p.vals.push_back(n);
      }
      ++i;
    }
    return shifted_input;
  }

  std::string part_2() override {
    uint64_t res        = 0;
    Input shifted_input = parse_shifted_input();
    for (const auto& p : shifted_input) {
      auto func = [&](auto a, auto b) {
        if (p.op == Op::ADD) {
          return a + b;
        } else {
          return a * b;
        }
      };

      uint64_t init = p.op == Op::ADD ? 0 : 1;
      res += std::accumulate(p.vals.begin(), p.vals.end(), init, func);
    }
    return std::to_string(res);
  }
};
};  // namespace AOC2025
