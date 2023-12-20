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

struct day_19 : public Advent_type {
  static constexpr int year            = 2023;
  static constexpr int date            = 19;
  const std::vector<std::string> input = read_lines<std::string>(year, date);

  enum class CategoryType {
    X,
    M,
    A,
    S
  };

  enum class CompareType {
    LT,
    GT
  };

  struct Rule {
    CategoryType category;
    CompareType compare;
    int value;
    std::string destWorkflow;
  };

  using CategoryMap_type = std::unordered_map<CategoryType, int>;

  struct Part {
    CategoryMap_type categoryMap;
    std::string currWorkflow = "in";
  };

  struct Workflow {
    std::vector<Rule> rules;
    std::string automaticDest;
  };

  using WorkflowMap_type = std::unordered_map<std::string, Workflow>;

  struct Input {
    WorkflowMap_type workflowMap;
    std::vector<Part> parts;
  };

  Input ParseInput() const {
    WorkflowMap_type workflowMap;
    std::vector<Part> parts;
    bool isSecondPartOfInput = false;
    for (const auto& v : input) {
      if (v.empty()) {
        isSecondPartOfInput = true;
        continue;
      }
      if (isSecondPartOfInput) {
        const auto vv = Strings::Split(v.substr(1, v.size() - 2), ",");
        CategoryMap_type categoryMap;
        for (const auto& vvv : vv) {
          const auto categoryAndValue = Strings::Split(vvv, "=");
          const auto& categoryStr     = categoryAndValue[0];
          const auto& valueStr        = categoryAndValue[1];
          const auto category         = [](const auto& categoryChar) {
            switch (categoryChar) {
              case 'x':
                return CategoryType::X;
              case 'm':
                return CategoryType::M;
              case 'a':
                return CategoryType::A;
              case 's':
                return CategoryType::S;
              default:
                throw std::runtime_error("Invalid categoryChar");
            }
          }(categoryStr[0]);
          categoryMap[category] = std::stoi(valueStr);
        }
        parts.emplace_back(categoryMap);
      } else {
        const auto vv          = Strings::Split(v, "{");
        const auto workflowStr = vv[0];
        std::vector<Rule> rules;
        const auto ruleStrs       = Strings::Split(vv[1].substr(0, vv[1].size() - 1), ",");
        std::string automaticDest = "";
        for (const auto& vvv : ruleStrs) {
          const auto conditionAndWorkflow = Strings::Split(vvv, ":");
          if (conditionAndWorkflow.size() != 1) {
            const auto& condition = conditionAndWorkflow[0];
            const auto& workflow  = conditionAndWorkflow[1];
            const auto category   = [](const auto& categoryChar) {
              switch (categoryChar) {
                case 'x':
                  return CategoryType::X;
                case 'm':
                  return CategoryType::M;
                case 'a':
                  return CategoryType::A;
                case 's':
                  return CategoryType::S;
                default:
                  throw std::runtime_error("Invalid categoryChar");
              }
            }(condition[0]);
            const auto compare = (condition[1] == '<') ? CompareType::LT : CompareType::GT;
            const auto value   = std::stoi(condition.substr(2));
            rules.emplace_back(category, compare, value, workflow);
          } else {
            automaticDest = vvv;
          }
        }
        workflowMap[workflowStr] = {rules, automaticDest};
      }
    }
    return {workflowMap, parts};
  }

  const Input inputStruct             = ParseInput();
  const WorkflowMap_type& workflowMap = inputStruct.workflowMap;
  const std::vector<Part>& parts      = inputStruct.parts;

  mutable uint32_t totalPartOne = 0;
  void Recurse(Part& part) const {
    if (part.currWorkflow == "A") {
      for (const auto& [k, v] : part.categoryMap) {
        totalPartOne += v;
      }
      return;
    }

    if (part.currWorkflow == "R") {
      return;
    }

    static const std::unordered_map<CompareType, bool (*)(int a, int b)> compareFunctionMap = {
        {CompareType::LT, [](int a, int b) { return a < b; }},
        {CompareType::GT, [](int a, int b) { return a > b; }}};
    const auto& workflow = workflowMap.at(part.currWorkflow);
    bool isAssigned      = false;
    for (const auto& rule : workflow.rules) {
      const auto& category        = rule.category;
      const auto& compareFunction = compareFunctionMap.at(rule.compare);
      const auto& partValue       = part.categoryMap.at(category);
      const auto& ruleValue       = rule.value;
      if (compareFunction(partValue, ruleValue)) {
        part.currWorkflow = rule.destWorkflow;
        isAssigned        = true;
        break;
      }
    }

    if (!isAssigned) {
      part.currWorkflow = workflow.automaticDest;
    }

    Recurse(part);
  }

  std::string part_1() override {
    auto partsCpy = parts;
    for (auto& part : partsCpy) {
      Recurse(part);
    }
    return std::to_string(totalPartOne);
  }

  using Pair_type                = std::pair<int, int>;
  using CategoryIntervalMap_type = std::unordered_map<CategoryType, Pair_type>;

  mutable uint64_t totalPartTwo = 0;
  void RecursePartTwo(const std::string& currWorkflow, CategoryIntervalMap_type intervalMap) const {
    if (currWorkflow == "A") {
      uint64_t numCombs = 1;
      for (const auto& [k, p] : intervalMap) {
        numCombs *= p.second - p.first + 1;
      }
      totalPartTwo += numCombs;
      return;
    }

    if (currWorkflow == "R") {
      return;
    }

    const auto& workflow = workflowMap.at(currWorkflow);
    for (const auto& rule : workflow.rules) {
      const auto& category     = rule.category;
      const auto& compare      = rule.compare;
      const auto& ruleValue    = rule.value;
      const auto& destWorkflow = rule.destWorkflow;
      if (compare == CompareType::GT) {
        auto& mapRange               = intervalMap[category];
        const auto& [a, b]           = mapRange;
        const Pair_type validRange   = {std::max(a, ruleValue + 1), b};
        const Pair_type invalidRange = {a, std::min(b, ruleValue)};
        if (validRange.first < validRange.second) {
          mapRange = validRange;
          RecursePartTwo(destWorkflow, intervalMap);
        }
        if (invalidRange.first < invalidRange.second) {
          mapRange = invalidRange;
        } else {
          return;
        }
      } else {
        auto& mapRange               = intervalMap[category];
        const auto& [a, b]           = mapRange;
        const Pair_type validRange   = {a, std::min(b, ruleValue - 1)};
        const Pair_type invalidRange = {std::max(ruleValue, a), b};
        if (validRange.first < validRange.second) {
          mapRange = validRange;
          RecursePartTwo(destWorkflow, intervalMap);
        }
        if (invalidRange.first < invalidRange.second) {
          mapRange = invalidRange;
        } else {
          return;
        }
      }
    }
    RecursePartTwo(workflow.automaticDest, intervalMap);
  }

  std::string part_2() override {
    const std::string startingWorkflow   = "in";
    static constexpr auto minValue = 1;
    static constexpr auto maxValue = 4000;
    CategoryIntervalMap_type intervalMap = {{CategoryType::X, {minValue, maxValue}},
                                            {CategoryType::M, {minValue, maxValue}},
                                            {CategoryType::A, {minValue, maxValue}},
                                            {CategoryType::S, {minValue, maxValue}}};
    RecursePartTwo(startingWorkflow, intervalMap);
    return std::to_string(totalPartTwo);
  }
};

};  // namespace AOC2023
