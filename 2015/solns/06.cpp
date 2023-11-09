/////////////////
//// std
/////////////////
#include <cstdint>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <string>
#include <unordered_set>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

namespace AOC2015 {

struct day_06 : public Advent_type {
  static constexpr int year = 2015;
  static constexpr int date = 6;

  enum class Action {
    kToggle,
    kTurnOff,
    kTurnOn
  };

  struct Instruct {
    Instruct(Action action, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
        : action(action), x1(x1), y1(y1), x2(x2), y2(y2) {}
    Action action;
    uint16_t x1;
    uint16_t y1;
    uint16_t x2;
    uint16_t y2;
  };

  const std::vector<Instruct> input = parse_input();

  std::vector<Instruct> parse_input() {
    const std::string regex_str =
        "\\b(toggle|turn off|turn on)\\s+(\\d+),(\\d+)\\s+through\\s+(\\d+),(\\d+)\\b";
    const auto rawInput = read_lines_vector_regex<std::string>(year, date, regex_str);
    std::vector<Instruct> vec;
    vec.reserve(rawInput.size());
    for (const auto& instruct : rawInput) {
      const auto& actionStr = instruct[0];
      const auto& x1        = static_cast<uint16_t>(std::stoi(instruct[1]));
      const auto& y1        = static_cast<uint16_t>(std::stoi(instruct[2]));
      const auto& x2        = static_cast<uint16_t>(std::stoi(instruct[3]));
      const auto& y2        = static_cast<uint16_t>(std::stoi(instruct[4]));
      Action action;
      if (actionStr == "toggle") {
        action = Action::kToggle;
      } else if (actionStr == "turn off") {
        action = Action::kTurnOff;
      } else if (actionStr == "turn on") {
        action = Action::kTurnOn;
      }
      vec.emplace_back(action, x1, y1, x2, y2);
    }

    return vec;
  }

  std::string part_1() override {
    static constexpr auto n        = 999;
    static constexpr auto gridSize = n * n;
    using Vector_type              = std::vector<bool>;
    using Ref_type                 = Vector_type::reference;
    Vector_type grid(gridSize, false);
    const auto getActionLambda = [](const auto& action) -> std::function<void(Ref_type)> {
      switch (action) {
        case (Action::kToggle):
          return [](Ref_type v) { v = !v; };
        case (Action::kTurnOff):
          return [](Ref_type v) { v = false; };
        case (Action::kTurnOn):
          return [](Ref_type v) { v = true; };
        default:
          std::unreachable();
      }
      std::unreachable();
    };
    for (const auto& instruct : input) {
      const auto actionLambda = getActionLambda(instruct.action);
      for (uint16_t y = instruct.y1; y <= instruct.y2; ++y) {
        for (uint16_t x = instruct.x1; x <= instruct.x2; ++x) {
          const auto index = x + y * n;
          actionLambda(grid[index]);
        }
      }
    }
    return std::to_string(std::accumulate(grid.begin(), grid.end(), 0));
  }

  std::string part_2() override {
    static constexpr auto n        = 999;
    static constexpr auto gridSize = n * n;
    std::vector<uint16_t> grid(gridSize, false);
    const auto getActionLambda = [](const auto& action) -> std::function<void(uint16_t&)> {
      switch (action) {
        case (Action::kToggle):
          return [](uint16_t& v) { v += 2; };
        case (Action::kTurnOff):
          return [](uint16_t& v) { v = std::max(0, v - 1); };
        case (Action::kTurnOn):
          return [](uint16_t& v) { ++v; };
        default:
          std::unreachable();
      }
      std::unreachable();
    };
    for (const auto& instruct : input) {
      const auto actionLambda = getActionLambda(instruct.action);
      for (uint16_t y = instruct.y1; y <= instruct.y2; ++y) {
        for (uint16_t x = instruct.x1; x <= instruct.x2; ++x) {
          const auto index = x + y * n;
          actionLambda(grid[index]);
        }
      }
    }
    return std::to_string(std::accumulate(grid.begin(), grid.end(), 0));
  }
};

};  // namespace AOC2015
