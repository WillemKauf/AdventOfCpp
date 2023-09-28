/////////////////
//// std
/////////////////
#include <algorithm>
#include <cstdint>
#include <limits>
#include <numeric>
#include <queue>
#include <unordered_map>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

struct day_09 : public Advent_type {
  static constexpr int year = 2015;
  static constexpr int date = 9;

  struct State {
    State(const std::string& node, int currDistance, uint8_t visited)
        : node(node), currDistance(currDistance), visited(visited) {}
    std::string node;
    int currDistance;
    uint8_t visited;

    auto operator<=>(const State& other) const {
      return std::tie(currDistance, other.visited) <=> std::tie(other.currDistance, visited);
    }
  };

  using Graph_type  = std::unordered_map<std::string, std::unordered_map<std::string, int>>;
  using Set_type    = std::unordered_set<std::string>;
  using Bitmap_type = std::unordered_map<std::string, uint8_t>;

  struct GraphStruct {
    Graph_type g;
    Set_type s;
    Bitmap_type b;
    uint8_t m;
  };

  GraphStruct makeGraph() {
    const std::vector<std::vector<std::string>> input =
        read_lines_vector_regex<std::string>(year, date, "(.+) to (.+) = (\\d+)");
    Graph_type g;
    Set_type s;
    Bitmap_type b;
    for (const auto& v : input) {
      const auto& a = v[0];
      const auto& b = v[1];
      const auto& d = std::stoi(v[2]);
      g[a][b]       = d;
      g[b][a]       = d;
      s.insert(a);
      s.insert(b);
    }

    uint8_t m = 0;
    for (const auto& node : s) {
      const uint8_t shift = 1 << b.size();
      b[node]             = shift;
      m |= shift;
    }

    return {g, s, b, m};
  }

  const GraphStruct graphStruct = makeGraph();
  const Graph_type& graph       = graphStruct.g;
  const Set_type& allNodes      = graphStruct.s;
  const Bitmap_type& bitMap     = graphStruct.b;
  const uint8_t& allSet         = graphStruct.m;

  template <typename Func_T>
  int traverseGrid() {
    std::priority_queue<State, std::vector<State>, Func_T> pq;
    for (const auto& [k, _] : graph) {
      pq.emplace(k, 0, bitMap.at(k));
    }

    static constexpr auto isMinHeap = std::is_same_v<Func_T, std::greater<State>>;
    int result = (isMinHeap) ? std::numeric_limits<int>::max() : std::numeric_limits<int>::min();

    while (!pq.empty()) {
      const auto curr = std::move(pq.top());
      pq.pop();
      if (curr.visited == allSet) {
        if constexpr (isMinHeap) {
          result = std::min(result, curr.currDistance);
        } else {
          result = std::max(result, curr.currDistance);
        }
        continue;
      }

      for (const auto& [neighbour, distance] : graph.at(curr.node)) {
        const auto neighbourBit = bitMap.at(neighbour);
        if (curr.visited & neighbourBit) {
          continue;
        }
        const auto newDistance = curr.currDistance + distance;
        if constexpr (isMinHeap) {
          if (newDistance >= result) {
            continue;
          }
        }
        pq.emplace(neighbour, newDistance, curr.visited | neighbourBit);
      }
    }
    return result;
  }

  std::string part_1() override { return std::to_string(traverseGrid<std::greater<State>>()); }

  std::string part_2() override { return std::to_string(traverseGrid<std::less<State>>()); }
};
