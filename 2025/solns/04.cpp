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

struct day_04 : public Advent_type {
  static constexpr int year = 2025;
  static constexpr int date = 4;
  using Vec_type            = std::vector<std::vector<char>>;
  Vec_type input            = read_lines_vector<char>(year, date);

  std::string part_1() override {
    uint64_t res = 0;
    for (int j = 0; j < input.size(); ++j) {
      for (int i = 0; i < input[j].size(); ++i) {
        if (input[j][i] == '.') {
          continue;
        }
        auto ddir      = Grid::GetAllCardinalDirs<Grid::GridDirection::WithDiagonals>();
        auto num_paper = 0;
        for (const auto& dd : ddir) {
          auto ii = i + dd[0];
          auto jj = j + dd[1];
          if (Grid::InBounds(ii, jj, input[j].size(), input.size())) {
            if (input[jj][ii] == '@') {
              ++num_paper;
            }
          }
        }
        if (num_paper < 4) {
          ++res;
        }
      }
    }
    return std::to_string(res);
  }

  std::string part_2() override {
    uint64_t res = 0;
    while (true) {
      uint64_t curr = 0;
      for (int j = 0; j < input.size(); ++j) {
        for (int i = 0; i < input[j].size(); ++i) {
          if (input[j][i] == '.') {
            continue;
          }
          auto ddir      = Grid::GetAllCardinalDirs<Grid::GridDirection::WithDiagonals>();
          auto num_paper = 0;
          for (const auto& dd : ddir) {
            auto ii = i + dd[0];
            auto jj = j + dd[1];
            if (Grid::InBounds(ii, jj, input[j].size(), input.size())) {
              if (input[jj][ii] == '@' || input[jj][ii] == 'x') {
                ++num_paper;
              }
            }
          }
          if (num_paper < 4) {
            input[j][i] = 'x';
            ++curr;
          }
        }
      }

      for (int j = 0; j < input.size(); ++j) {
        for (int i = 0; i < input[j].size(); ++i) {
          if (input[j][i] == 'x') {
            input[j][i] = '.';
          }
        }
      }

      if (curr == 0) {
        break;
      }

      res += curr;
    }
    return std::to_string(res);
  }
};

};  // namespace AOC2025
