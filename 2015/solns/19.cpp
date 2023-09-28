/////////////////
//// std
/////////////////
#include <limits>
#include <queue>
#include <ranges>
#include <stdexcept>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

struct day_19 : public Advent_type {
  static constexpr int year = 2015;
  static constexpr int date = 19;

  struct cmpByStringLength {
    bool operator()(const std::string& a, const std::string& b) const {
      const auto aSize = a.size();
      const auto bSize = b.size();
      return std::tie(aSize, a) > std::tie(bSize, b);
    }
  };

  using Map_type =
      std::map<std::string, std::set<std::string, cmpByStringLength>, cmpByStringLength>;
  // using Map_type = std::unordered_map<std::string, std::unordered_set<std::string>>;

  struct Input {
    Map_type replacementMap;
    std::string medicineMolecule;
  };

  template <bool reverseMap = false>
  Input parseInput() {
    const std::vector<std::vector<std::string>> input =
        read_lines_vector_regex<std::string>(year, date, "^(\\w+)\\s*=>\\s*(\\w+)$", "(\\w+)");
    Map_type rMap;
    std::string mMolecule;
    for (const auto& v : input) {
      if (v.empty()) {
        continue;
      }
      if (v.size() == 2) {
        if constexpr (reverseMap) {
          rMap[v[1]].insert(v[0]);
        } else {
          rMap[v[0]].insert(v[1]);
        }
      } else if (v.size() == 1) {
        mMolecule = v[0];
      }
    }
    return {rMap, mMolecule};
  }

  auto getEnumeratedMolecule(const std::string& molecule, const Map_type& replacementMap) const {
    auto s = molecule | std::views::enumerate |
             std::views::chunk_by([&](const auto& a, const auto& b) {
               const auto& [iA, cA] = a;
               const auto& [iB, cB] = b;
               return replacementMap.contains(std::string{cA} + std::string{cB});
             }) |
             std::views::transform([](const auto& chunk) -> std::pair<int, std::string> {
               return std::pair<int, std::string>{
                   std::get<0>(*chunk.begin()),
                   std::accumulate(chunk.begin(), chunk.end(), std::string{},
                                   [](const std::string& str, const auto& pair) {
                                     return str + std::get<1>(pair);
                                   })};
             }) |
             std::views::filter(
                 [&](const auto& molPair) { return replacementMap.contains(molPair.second); });
    return s;
  }

  std::string part_1() override {
    const Input input                   = parseInput();
    const Map_type& replacementMap      = input.replacementMap;
    const std::string& medicineMolecule = input.medicineMolecule;

    std::unordered_set<std::string> moleculeSet;
    for (const auto& [index, mol] : getEnumeratedMolecule(medicineMolecule, replacementMap)) {
      for (const auto& repMol : replacementMap.at(mol)) {
        const auto newMolecule = medicineMolecule.substr(0, index) + repMol +
                                 medicineMolecule.substr(index + mol.size());
        moleculeSet.insert(std::move(newMolecule));
      }
    }
    return std::to_string(moleculeSet.size());
  }

  struct State {
    State(const std::string& currMolecule, int numSteps)
        : currMolecule(currMolecule), numSteps(numSteps) {}
    std::string currMolecule;
    int numSteps;

    auto operator<=>(const State& other) const {
      const auto molSize      = currMolecule.size();
      const auto otherMolSize = other.currMolecule.size();
      return std::tie(molSize, numSteps) <=> std::tie(otherMolSize, other.numSteps);
    }
  };

  std::string part_2() override {
    //const Input input                   = parseInput<true>();
    //const Map_type& replacementMap      = input.replacementMap;
    //const std::string& medicineMolecule = input.medicineMolecule;
    //// std::priority_queue<State, std::vector<State>, std::greater<State>> pq;
    //auto molecule = medicineMolecule;
    //while (molecule != "e") {
    //  std::cout << molecule << '\n';
    //  for (const auto& [mol, repSet] : replacementMap) {
    //    for (const auto& repMol : repSet) {
    //      int searchIndex = molecule.find(mol);
    //      if (searchIndex != std::string::npos) {
    //        molecule = molecule.substr(0, searchIndex) + repMol +
    //                   molecule.substr(searchIndex + mol.size());
    //      }
    //    }
    //  }
    //}
    // pq.emplace(medicineMolecule, 0);
    // int minSteps              = std::numeric_limits<int>::max();
    // const std::string destMol = "e";
    // std::unordered_set<std::string> seen;
    // while (!pq.empty()) {
    //   const auto curr = std::move(pq.top());
    //   pq.pop();
    //   if (curr.currMolecule == destMol) {
    //     minSteps = std::min(minSteps, curr.numSteps);
    //     continue;
    //   }
    //   if (curr.numSteps >= minSteps) {
    //     continue;
    //   }
    //   if (seen.contains(curr.currMolecule)) {
    //     continue;
    //   }
    //   std::cout << curr.currMolecule << '\n';
    //   seen.insert(curr.currMolecule);
    //   auto newMolecule = curr.currMolecule;
    //   for (const auto& [mol, repSet] : replacementMap) {
    //     for (const auto& repMol : repSet) {
    //       int searchIndex = newMolecule.find(mol);
    //       if (searchIndex != std::string::npos) {
    //         newMolecule = newMolecule.substr(0, searchIndex) + repMol +
    //                       newMolecule.substr(searchIndex + mol.size());
    //       }
    //     }
    //   }
    //   pq.emplace(std::move(newMolecule), curr.numSteps + 1);
    // }
    // return std::to_string(minSteps);
    return "Unimplemented.";
  }
};
