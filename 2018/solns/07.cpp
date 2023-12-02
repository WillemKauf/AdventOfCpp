/////////////////
//// std
/////////////////
#include <queue>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2018 {

struct day_07 : public Advent_type {
  static constexpr int year                  = 2018;
  static constexpr int date                  = 7;
  const std::vector<std::vector<char>> input = read_lines_vector_regex<char>(
      year, date, "Step (\\w+) must be finished before step (\\w+) can begin.");

  using Graph_type   = std::unordered_map<char, std::unordered_set<char>>;
  using Degrees_type = std::unordered_map<char, int>;

  struct GraphInput {
    Graph_type graph;
    Degrees_type inDegrees;
  };

  GraphInput ParseInput() const {
    Graph_type graph;
    Degrees_type inDegrees;
    for (const auto& v : input) {
      const auto& a = v[0];
      const auto& b = v[1];
      graph[a].insert(b);
      graph[b];
      inDegrees[a];
      ++inDegrees[b];
    }
    return {graph, inDegrees};
  }

  const GraphInput graphInput   = ParseInput();
  const Graph_type& graph       = graphInput.graph;
  const Degrees_type& inDegrees = graphInput.inDegrees;

  std::string part_1() override {
    using Queue_type = std::priority_queue<char, std::vector<char>, std::greater<char>>;

    Queue_type q;
    auto inDs = inDegrees;

    for (const auto& [node, inDegree] : inDs) {
      if (inDegree == 0) {
        q.push(node);
      }
    }

    std::string stepStr;
    while (!q.empty()) {
      const auto curr = std::move(q.top());
      q.pop();
      stepStr += curr;
      for (const auto& n : graph.at(curr)) {
        if (--inDs.at(n) == 0) {
          q.push(n);
        }
      }
    }

    return stepStr;
  }

  std::string part_2() override {
    static constexpr auto numWorkers = 5;
    static constexpr auto stepTime   = 60;
    using Task                       = std::pair<int, char>;
    using Queue_type     = std::priority_queue<char, std::vector<char>, std::greater<char>>;
    using WorkQueue_type = std::priority_queue<Task, std::vector<Task>, std::greater<Task>>;

    Queue_type Q;
    WorkQueue_type workQ;
    auto inDs = inDegrees;

    for (const auto& [node, inDegree] : inDs) {
      if (inDegree == 0) {
        Q.emplace(node);
      }
    }

    int t = 0;
    while (!Q.empty() || !workQ.empty()) {
      while (workQ.size() < numWorkers && !Q.empty()) {
        const auto nextStep = std::move(Q.top());
        Q.pop();
        workQ.emplace(t + (nextStep - 'A') + 1 + stepTime, nextStep);
      }

      const auto nextStep = std::move(workQ.top());
      workQ.pop();
      t = nextStep.first;

      for (const auto& n : graph.at(nextStep.second)) {
        if (--inDs.at(n) == 0) {
          Q.emplace(n);
        }
      }
    }

    return std::to_string(t);
  }
};

};  // namespace AOC2018
