#include <cstddef>
#include <cstdint>
#include <numeric>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"
#include "algorithm.h"
#include "data_structure.h"
#include "grid.h"

namespace AOC2025 {

struct day_08 : public Advent_type {
  static constexpr int year = 2025;
  static constexpr int date = 8;
  using Vec_type            = std::vector<std::vector<uint32_t>>;
  const Vec_type input      = read_lines_vector_regex<uint32_t>(year, date, "(\\d+)");

  struct Box {
    size_t n;
    uint32_t x;
    uint32_t y;
    uint32_t z;
    auto operator<=>(const Box& o) const = default;
  };

  uint64_t sqr_dist(const Box& a, const Box& b) {
    auto dx = static_cast<int64_t>(b.x) - static_cast<int64_t>(a.x);
    auto dy = static_cast<int64_t>(b.y) - static_cast<int64_t>(a.y);
    auto dz = static_cast<int64_t>(b.z) - static_cast<int64_t>(a.z);
    return dx * dx + dy * dy + dz * dz;
  }

  using Boxes_type = std::vector<Box>;

  Boxes_type ParseInput() {
    Boxes_type boxes;
    boxes.reserve(input.size());
    for (auto [i, v] : std::views::enumerate(input)) {
      boxes.emplace_back(i, v[0], v[1], v[2]);
    }
    return boxes;
  }

  Boxes_type initial_boxes = ParseInput();

  uint32_t make_pair_key(uint32_t a, uint32_t b, uint32_t n) {
    if (a > b) {
      std::swap(a, b);
    }
    return a + b * n;
  }

  struct Distance {
    uint64_t dist;
    uint32_t na;
    uint32_t nb;

    auto operator<=>(const Distance& o) const { return dist <=> o.dist; }
  };

  std::vector<Distance> GetDistances() {
    std::vector<Distance> distances;
    distances.reserve(initial_boxes.size() * (initial_boxes.size() - 1) / 2);
    std::unordered_set<uint32_t> seen_pairs;
    for (auto& a : initial_boxes) {
      for (auto& b : initial_boxes) {
        if (a == b) {
          continue;
        }
        auto pair = make_pair_key(a.n, b.n, initial_boxes.size());
        if (seen_pairs.contains(pair)) {
          continue;
        }
        seen_pairs.insert(pair);
        distances.emplace_back(sqr_dist(a, b), a.n, b.n);
      }
    }
    std::sort(distances.begin(), distances.end());
    return distances;
  }

  std::vector<Distance> distances = GetDistances();

  std::string part_1() override {
    auto boxes = initial_boxes;
    DataStructure::DSU dsu(boxes.size());
    static constexpr auto num_connections = 1000;
    for (int i = 0; i < num_connections; ++i) {
      const auto& c = distances[i];
      dsu.JoinRoots(c.na, c.nb);
    }

    std::priority_queue<uint64_t> pq;
    for (const auto& root : dsu.GetRoots()) {
      pq.push(dsu.GetSize(root));
    }

    uint64_t res = 1;
    for (int i = 0; i < 3; ++i) {
      res *= pq.top();
      pq.pop();
    }

    return std::to_string(res);
  }

  std::string part_2() override {
    DataStructure::DSU dsu(initial_boxes.size());
    for (const auto& c : distances) {
      dsu.JoinRoots(c.na, c.nb);
      if (dsu.GetNumSets() == 1) {
        auto ax      = static_cast<uint64_t>(initial_boxes[c.na].x);
        auto bx      = static_cast<uint64_t>(initial_boxes[c.nb].x);
        uint64_t res = ax * bx;
        return std::to_string(res);
      }
    }

    std::unreachable();
  }
};

};  // namespace AOC2025
