/////////////////
//// std
/////////////////
#include <algorithm>
#include <bitset>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

namespace AOC2016 {

struct day_08 : public Advent_type {
  static constexpr int year                         = 2016;
  static constexpr int date                         = 8;
  const std::vector<std::vector<std::string>> input = read_lines_vector_regex<std::string>(
      year, date, "rotate.*(\\w)=(\\d+) by (\\d+)", "rect (\\d+)x(\\d+)");

  static constexpr int width  = 50;
  static constexpr int height = 6;

  enum class Instruct {
    RECT,
    ROTATEROW,
    ROTATECOL
  };

  struct Instruction {
    Instruction(Instruct instruct, int a, int b) : instruct(instruct), a(a), b(b) {}
    Instruct instruct;
    int a;
    int b;
  };

  std::vector<Instruction> ParseInput() {
    std::vector<Instruction> is;
    is.reserve(input.size());
    for (const auto& vec : input) {
      switch (vec.size()) {
        case 2:
          is.emplace_back(Instruct::RECT, std::stoi(vec[0]), std::stoi(vec[1]));
          break;
        case 3:
          is.emplace_back((vec[0] == "x") ? Instruct::ROTATECOL : Instruct::ROTATEROW,
                          std::stoi(vec[1]), std::stoi(vec[2]));
          break;
        default:
          throw std::runtime_error("Invalid vec size in ParseInput");
      }
    }
    return is;
  }

  const std::vector<Instruction> instructs = ParseInput();

  template <typename T>
  void Print(const T& t) {
    for (const auto& v : t) {
      for (const auto& s : v) {
        auto c = (s == true) ? "⬛" : "⬜";
        std::cout << c;
      }
      std::cout << '\n';
    }
    std::cout << '\n';
  }

  std::string part_1() override {
    auto grid = std::vector<std::vector<bool>>(height, std::vector<bool>(width, false));
    for (const auto& instruct : instructs) {
      switch (instruct.instruct) {
        case Instruct::RECT: {
          for (int j = 0; j < instruct.b; ++j) {
            for (int i = 0; i < instruct.a; ++i) {
              grid[j][i] = true;
            }
          }
          break;
        }
        case Instruct::ROTATEROW: {
          auto& row         = grid[instruct.a];
          const auto& shift = instruct.b;
          auto newRow       = std::vector<bool>(width, false);
          for (auto [index, c] : std::views::enumerate(row)) {
            if (c) {
              auto newIndex    = (index + shift) % width;
              newRow[newIndex] = c;
            }
          }
          std::swap(row, newRow);
          break;
        }
        case Instruct::ROTATECOL: {
          std::unordered_set<int> indices;
          const auto col    = instruct.a;
          const auto& shift = instruct.b;
          for (int j = 0; j < height; ++j) {
            if (grid[j][col]) {
              const auto newIndex = (j + shift) % height;
              indices.insert(newIndex);
            }
            grid[j][col] = false;
          }
          for (const auto& index : indices) {
            grid[index][col] = true;
          }
          break;
        }
        default:
          throw std::runtime_error("Invalid Instruct");
      }
    }
    Print(grid);
    return std::to_string(std::accumulate(
        grid.begin(), grid.end(), 0,
        [](int cnt, const auto& vec) { return cnt + std::count(vec.begin(), vec.end(), true); }));
  }

  std::string part_2() override { return "Please refer to output."; }
};

};  // namespace AOC2016
