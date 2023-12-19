/////////////////
//// std
/////////////////
#include <cstdint>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2023 {

struct day_16 : public Advent_type {
  static constexpr int year            = 2023;
  static constexpr int date            = 16;
  const std::vector<std::string> input = read_lines<std::string>(year, date);

  using Int_type  = int8_t;
  using Pair_type = std::pair<Int_type, Int_type>;
  using Hash_type = uint32_t;

  static constexpr auto sizeOfInt = sizeof(Int_type) * 8;

  struct State {
    State(const Pair_type& pos, const Pair_type& dir) : pos(pos), dir(dir) {}
    Pair_type pos;
    Pair_type dir;
  };

  int PathFind(const Pair_type& startPos, const Pair_type& startDir) const {
    const auto m = input.size();
    const auto n = input[0].size();

    const auto hashFunc = [&](const auto& pos, const auto& dir) -> Hash_type {
      Hash_type hash = {};
      hash |= (pos.first);
      hash |= (static_cast<Hash_type>(pos.second) << sizeOfInt);
      hash |= (static_cast<Hash_type>(dir.first) << (sizeOfInt * 2));
      hash |= (static_cast<Hash_type>(dir.second) << (sizeOfInt * 3));
      return hash;
    };

    const auto hashFuncPos = [&](const auto& pos) -> Hash_type {
      Hash_type hash = {};
      hash |= (pos.first);
      hash |= (static_cast<Hash_type>(pos.second) << sizeOfInt);
      return hash;
    };

    std::unordered_set<Hash_type> seen;
    std::unordered_set<Hash_type> seenPos;

    std::queue<State> q;
    q.emplace(startPos, startDir);
    while (!q.empty()) {
      const auto curr = std::move(q.front());
      q.pop();
      const auto& pos = curr.pos;
      const auto& dir = curr.dir;
      const auto hash = hashFunc(pos, dir);
      if (seen.contains(hash)) {
        continue;
      }
      seen.insert(hash);
      const auto hashPos = hashFuncPos(pos);
      seenPos.insert(hashPos);
      const auto ii = pos.first + dir.first;
      const auto jj = pos.second + dir.second;
      if (Grid::InBounds(ii, jj, n, m)) {
        const auto& c = input[jj][ii];
        switch (c) {
          case '.': {
            q.emplace(Pair_type{ii, jj}, dir);
            break;
          }
          case '|': {
            if (!dir.first) {
              // Moving vertically
              q.emplace(Pair_type{ii, jj}, dir);
            } else {
              // Moving horizontally, push back two beams
              q.emplace(Pair_type{ii, jj}, Pair_type{0, -1});
              q.emplace(Pair_type{ii, jj}, Pair_type{0, 1});
            }
            break;
          }
          case '-': {
            if (!dir.second) {
              // Moving horizontally
              q.emplace(Pair_type{ii, jj}, dir);
            } else {
              // Moving vertically, push back two beams
              q.emplace(Pair_type{ii, jj}, Pair_type{-1, 0});
              q.emplace(Pair_type{ii, jj}, Pair_type{1, 0});
            }
            break;
          }
          case '\\': {
            if (!dir.second) {
              // Moving horizontally
              if (dir.first > 0) {
                // Approaching from left
                q.emplace(Pair_type{ii, jj}, Pair_type{0, 1});
              } else {
                // Approaching from right
                q.emplace(Pair_type{ii, jj}, Pair_type{0, -1});
              }
            } else {
              // Moving vertically
              if (dir.second > 0) {
                // Approaching from top
                q.emplace(Pair_type{ii, jj}, Pair_type{1, 0});
              } else {
                // Approaching from bottom
                q.emplace(Pair_type{ii, jj}, Pair_type{-1, 0});
              }
            }
            break;
          }
          case '/': {
            if (!dir.second) {
              // Moving horizontally
              if (dir.first > 0) {
                // Approaching from left
                q.emplace(Pair_type{ii, jj}, Pair_type{0, -1});
              } else {
                // Approaching from right
                q.emplace(Pair_type{ii, jj}, Pair_type{0, 1});
              }
            } else {
              // Moving vertically
              if (dir.second > 0) {
                // Approaching from top
                q.emplace(Pair_type{ii, jj}, Pair_type{-1, 0});
              } else {
                // Approaching from bottom
                q.emplace(Pair_type{ii, jj}, Pair_type{1, 0});
              }
            }
            break;
          }
          default:
            throw std::runtime_error("Unhandled char");
        }
      }
    }
    return seenPos.size() - 1;
  }

  std::string part_1() override { return std::to_string(PathFind({-1, 0}, {1, 0})); }

  std::string part_2() override {
    const auto m = input.size();
    const auto n = input[0].size();
    int mx       = std::numeric_limits<int>::min();
    for (int j = 0; j < m; ++j) {
      mx = std::max(mx, PathFind({-1, j}, {1, 0}));
      mx = std::max(mx, PathFind({n, j}, {-1, 0}));
    }
    for (int i = 0; i < n; ++i) {
      mx = std::max(mx, PathFind({i, -1}, {0, 1}));
      mx = std::max(mx, PathFind({i, m}, {0, -1}));
    }
    return std::to_string(mx);
  }
};

};  // namespace AOC2023
