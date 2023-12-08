/////////////////
//// std
/////////////////

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2023 {

struct day_08 : public Advent_type {
  static constexpr int year = 2023;
  static constexpr int date = 8;
  const std::vector<std::vector<std::string>> input =
      read_lines_vector_regex<std::string>(year, date, "(\\w+) = \\((\\w+), (\\w+)\\)", "(\\w+)");

  using Node_type  = std::string;
  using Graph_type = std::unordered_map<Node_type, std::pair<Node_type, Node_type>>;

  struct Input {
    Graph_type graph;
    std::string instruct;
  };

  Input ParseInput() const {
    Graph_type graph;
    const auto instruct = input[0][0];
    for (const auto& v : input) {
      if (v.size() == 3) {
        graph[v[0]] = std::make_pair(v[1], v[2]);
      }
    }
    return {graph, instruct};
  }

  const Input inputStruct     = ParseInput();
  const Graph_type& graph     = inputStruct.graph;
  const std::string& instruct = inputStruct.instruct;

  std::string part_1() override {
    struct State {
      Node_type node;
      int numSteps;
    };

    auto state                          = State{"AAA", 0};
    static constexpr Node_type goalNode = "ZZZ";
    while (state.node != goalNode) {
      const auto& [l, r] = graph.at(state.node);
      const auto& LR     = instruct[state.numSteps % instruct.size()];
      state.node         = (LR == 'L') ? l : r;
      ++state.numSteps;
    }

    return std::to_string(state.numSteps);
  }

  std::string part_2() override {
    struct State {
      using Int_type = uint64_t;
      State(const std::vector<Node_type>& nodes, int numSteps)
          : nodes(nodes), LCM(nodes.size(), -1), numSteps(numSteps) {}
      std::vector<Node_type> nodes;
      std::vector<Int_type> LCM;
      int numSteps;
    };

    std::vector<Node_type> start;
    for (const auto& [k, v] : graph) {
      if (k.back() == 'A') {
        start.push_back(k);
      }
    }

    auto state = State(start, 0);
    while (
        !(std::all_of(state.LCM.begin(), state.LCM.end(), [](const auto& i) { return i != -1; }))) {
      for (auto [index, node] : std::views::enumerate(state.nodes)) {
        if (node.back() == 'Z') {
          state.LCM[index] = state.numSteps;
        }
        const auto& [l, r] = graph.at(node);
        const auto& LR     = instruct[state.numSteps % instruct.size()];
        node               = (LR == 'L') ? l : r;
      }
      ++state.numSteps;
    }

    return std::to_string(Algorithm::LCM(state.LCM));
  }
};

};  // namespace AOC2023
