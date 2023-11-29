/////////////////
//// std
/////////////////
#include <cctype>
#include <string>
#include <vector>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2017 {

struct day_19 : public Advent_type {
  static constexpr int year            = 2017;
  static constexpr int date            = 19;
  const std::vector<std::string> input = read_lines<std::string>(year, date);

  int numSteps = 0;

  struct State {
    using Pair_type = std::pair<int, int>;
    State(const Pair_type& pos, const Pair_type& dir) : pos(pos), dir(dir) {}

    Pair_type pos;
    Pair_type dir;
  };

  std::string part_1() override {
    static constexpr auto ddir = Grid::GetAllCardinalDirs();
    const int m                = input.size();
    const int n                = input[0].size();
    int startingIndex          = {};
    for (auto [index, v] : std::views::enumerate(input[0])) {
      if (v != ' ') {
        startingIndex = index;
      }
    }
    std::queue<State> q;
    q.emplace(State::Pair_type{startingIndex, 0}, State::Pair_type{0, 1});
    std::string path = "";
    while (!q.empty()) {
      const auto currState = std::move(q.front());
      q.pop();
      ++numSteps;
      const auto& i = currState.pos.first;
      const auto& j = currState.pos.second;
      auto di       = currState.dir.first;
      auto dj       = currState.dir.second;
      const auto& c = input[j][i];

      if (c == ' ') {
        --numSteps;
        break;
      }

      if (std::isalpha(c)) {
        path += c;
      }

      if (c == '+') {
        for (const auto& dd : ddir) {
          const auto& ii = i + dd[0];
          const auto& jj = j + dd[1];
          if ((dd[0] != 0 && dd[0] == -di) || (dd[1] != 0 && dd[1] == -dj)) {
            continue;
          }
          if (0 <= ii && ii < n && 0 <= jj && jj < m) {
            if (input[jj][ii] != ' ') {
              di = dd[0];
              dj = dd[1];
              break;
            }
          }
        }
      }
      const auto ii = i + di;
      const auto jj = j + dj;
      q.emplace(State::Pair_type{ii, jj}, State::Pair_type{di, dj});
    }

    return path;
  }

  std::string part_2() override { return std::to_string(numSteps); }
};

};  // namespace AOC2017
