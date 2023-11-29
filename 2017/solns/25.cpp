/////////////////
//// std
/////////////////
#include <cstdint>
#include <unordered_map>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2017 {

struct day_25 : public Advent_type {
  static constexpr int year = 2017;
  static constexpr int date = 25;

  // Going to hard code the input. Writing a parser is not worth it.
  static constexpr int numSteps  = 12629077;
  static constexpr int numStates = 6;

  struct Action {
    uint8_t writeValue;
    int8_t moveDir;
    int nextState;
  };

  struct State {
    Action zeroAction;
    Action oneAction;
  };

  struct TapeState {
    State state;
    int pos;
  };

  static constexpr std::array<State, numStates> stateMap = {
      State{Action{1, 1, 1},  Action{0, -1, 1}}, State{Action{0, 1, 2},  Action{1, -1, 1}},
      State{Action{1, 1, 3},  Action{0, -1, 0}}, State{Action{1, -1, 4}, Action{1, -1, 5}},
      State{Action{1, -1, 0}, Action{0, -1, 3}}, State{Action{1, 1, 0},  Action{1, -1, 4}}};

  std::string part_1() override {
    std::unordered_map<int, uint8_t> tape;
    auto tapeState = TapeState{stateMap[0], 0};
    for (int t = 0; t < numSteps; ++t) {
      auto& p       = tapeState.pos;
      auto& c       = tape[p];
      auto& s       = tapeState.state;
      const auto& a = (c == 0) ? s.zeroAction : s.oneAction;
      c             = a.writeValue;
      p += a.moveDir;
      s = stateMap.at(a.nextState);
    }

    return std::to_string(std::accumulate(tape.begin(), tape.end(), 0,
                                          [](int sm, const auto& p) { return sm + p.second; }));
  }

  std::string part_2() override { return DayTwentyFiveString; }
};

};  // namespace AOC2017
