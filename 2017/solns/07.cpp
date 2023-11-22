/////////////////
//// std
/////////////////
#include <unordered_map>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2017 {

struct day_07 : public Advent_type {
  static constexpr int year = 2017;
  static constexpr int date = 7;
  const std::vector<std::vector<std::string>> input =
      read_lines_vector_regex<std::string>(year, date, "(\\w+)");

  using Graph_type    = std::unordered_map<std::string, std::unordered_set<std::string>>;
  using ValueMap_type = std::unordered_map<std::string, int>;

  struct State {
    State(Graph_type&& graph, ValueMap_type&& valueMap)
        : graph(std::move(graph)), valueMap(std::move(valueMap)) {}
    Graph_type graph;
    ValueMap_type valueMap;
  };

  State ParseInput() const {
    Graph_type g;
    ValueMap_type vMap;
    for (const auto& v : input) {
      const auto& root = v[0];
      const auto value = std::stoi(v[1]);
      vMap.insert_or_assign(root, value);
      if (v.size() > 2) {
        for (auto child : std::views::drop(v, 2)) {
          g[root].insert(child);
        }
      }
    }
    return {std::move(g), std::move(vMap)};
  };

  State inputState              = ParseInput();
  Graph_type& graph             = inputState.graph;
  const ValueMap_type& valueMap = inputState.valueMap;

  std::string rootNode = {};

  std::string part_1() override {
    std::unordered_set<std::string> childNodeSet;
    for (const auto& [parent, childSet] : graph) {
      for (const auto& child : childSet) {
        childNodeSet.insert(child);
      }
    }

    for (const auto& [parent, childSet] : graph) {
      if (!childNodeSet.contains(parent)) {
        return rootNode = parent;
      }
    }
    std::unreachable();
  }

  int GetSum(const std::string& node) {
    int sm = valueMap.at(node);
    for (const auto& child : graph[node]) {
      sm += GetSum(child);
    }
    return sm;
  }

  std::optional<int> CheckNode(const std::string& node) {
    std::unordered_map<int, std::unordered_set<std::string>> nodeSums;

    for (const auto& child : graph[node]) {
      nodeSums[GetSum(child)].insert(child);
    }

    int uniqueSum = -1;
    int otherSum  = -1;
    std::string uniqueNode;

    for (const auto& [nodeSum, nodeSet] : nodeSums) {
      if (nodeSet.size() == 1) {
        uniqueSum  = nodeSum;
        uniqueNode = *nodeSet.begin();
      } else {
        otherSum = nodeSum;
      }
    }

    if (uniqueSum == -1) {
      return std::nullopt;
    } else {
      const auto dfsWeight = CheckNode(uniqueNode);
      return (dfsWeight.has_value()) ? dfsWeight : valueMap.at(uniqueNode) + otherSum - uniqueSum;
    }
  }

  std::string part_2() override { return std::to_string(CheckNode(rootNode).value()); }
};

};  // namespace AOC2017
