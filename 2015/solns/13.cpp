/////////////////
//// std
/////////////////
#include <limits>
#include <queue>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

namespace AOC2015 {

struct day_13 : public Advent_type {
  static constexpr int year = 2015;
  static constexpr int date = 13;

  using Graph_type  = std::unordered_map<std::string, std::unordered_map<std::string, int>>;
  using Set_type    = std::unordered_set<std::string>;
  using Bitmap_type = std::unordered_map<std::string, uint16_t>;

  struct GraphStruct {
    Graph_type g;
    Set_type s;
    Bitmap_type b;
    uint16_t m;
  };

  GraphStruct makeGraph() const {
    const std::vector<std::vector<std::string>> input = read_lines_vector_regex<std::string>(
        year, date, R"((\w+) would (gain|lose) (\d+) happiness units by sitting next to (\w+)\.)");
    Graph_type g;
    Set_type s;
    Bitmap_type b;
    for (const auto& v : input) {
      const auto& personA = v[0];
      const auto& sign    = v[1];
      const auto& mag     = std::stoi(v[2]);
      const auto& personB = v[3];
      const auto value    = (sign == "gain") ? mag : -mag;
      g[personA][personB] = value;
      s.insert(personA);
      s.insert(personB);
    }

    uint16_t m = 0;
    for (const auto& node : s) {
      const uint16_t shift = 1 << b.size();
      b[node]              = shift;
      m |= shift;
    }

    return {g, s, b, m};
  }

  GraphStruct graphStruct = makeGraph();
  Graph_type& graph       = graphStruct.g;
  Set_type& allNodes      = graphStruct.s;
  Bitmap_type& bitMap     = graphStruct.b;
  uint16_t& allSet        = graphStruct.m;

  struct State {
    State(const std::string& prevPerson, const std::string& leftMostPerson, int currHappiness,
          int index, uint16_t usedPeople)
        : prevPerson(prevPerson),
          leftMostPerson(leftMostPerson),
          currHappiness(currHappiness),
          index(index),
          usedPeople(usedPeople) {}

    std::string prevPerson;
    std::string leftMostPerson;
    int currHappiness;
    int index;
    uint16_t usedPeople;

    auto operator<=>(const State& other) const { return currHappiness <=> other.currHappiness; }
  };

  std::string traverseGrid() const {
    std::priority_queue<State> pq;

    for (const auto& node : allNodes) {
      pq.emplace(node, node, 0, 1, bitMap.at(node));
    }

    const auto n     = allNodes.size();
    int maxHappiness = std::numeric_limits<int>::min();
    while (!pq.empty()) {
      const auto curr = std::move(pq.top());
      pq.pop();
      if (curr.index == n) {
        maxHappiness = std::max(
            maxHappiness, curr.currHappiness + graph.at(curr.leftMostPerson).at(curr.prevPerson) +
                              graph.at(curr.prevPerson).at(curr.leftMostPerson));
        continue;
      }
      for (const auto& [neighbor, value] : graph.at(curr.prevPerson)) {
        const auto neighborMask = bitMap.at(neighbor);
        if (curr.usedPeople & neighborMask) continue;
        pq.emplace(neighbor, curr.leftMostPerson,
                   curr.currHappiness + value + graph.at(neighbor).at(curr.prevPerson),
                   curr.index + 1, curr.usedPeople | neighborMask);
      }
    }
    return std::to_string(maxHappiness);
  }

  std::string part_1() override { return traverseGrid(); }

  std::string part_2() override {
    const auto youStr = "You";
    for (const auto& node : allNodes) {
      graph[node][youStr] = 0;
      graph[youStr][node] = 0;
    }
    allNodes.insert(youStr);
    const auto youShift = 1 << bitMap.size();
    bitMap[youStr]      = youShift;
    allSet |= youShift;
    return traverseGrid();
  }
};

};  // namespace AOC2015
