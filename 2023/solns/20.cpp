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

struct day_20 : public Advent_type {
  static constexpr int year            = 2023;
  static constexpr int date            = 20;
  const std::vector<std::string> input = read_lines<std::string>(year, date);

  enum class ModuleType {
    NONE,
    FLIPFLOP,
    CONJUNCTION
  };

  enum class PulseType {
    LOW,
    HIGH
  };

  struct Module {
    ModuleType moduleType;
    bool state                                          = false;
    std::unordered_map<std::string, PulseType> pulseMap = {};
  };

  using ModuleMap_type     = std::unordered_map<std::string, Module>;
  using ConnectionMap_type = std::unordered_map<std::string, std::vector<std::string>>;

  struct Input {
    ModuleMap_type moduleMap;
    ConnectionMap_type connectionMap;
  };

  Input ParseInput() const {
    ModuleMap_type moduleMap;
    ConnectionMap_type connectionMap;
    for (const auto& v : input) {
      auto split                = Strings::Split(v, " -> ");
      auto& parent              = split[0];
      const auto& connectedList = split[1];
      auto moduleType           = ModuleType::NONE;
      if (parent[0] == '%') {
        parent     = parent.substr(1);
        moduleType = ModuleType::FLIPFLOP;
      } else if (parent[0] == '&') {
        parent     = parent.substr(1);
        moduleType = ModuleType::CONJUNCTION;
      }
      moduleMap[parent] = {moduleType};
      for (const auto& connected : Strings::Split(connectedList, ", ")) {
        connectionMap[parent].push_back(connected);
      }
    }

    for (const auto& [k, v] : connectionMap) {
      for (const auto& vv : v) {
        if (auto moduleMapIt = moduleMap.find(vv); moduleMapIt != moduleMap.end()) {
          auto& childModule = moduleMapIt->second;
          if (childModule.moduleType == ModuleType::CONJUNCTION) {
            childModule.pulseMap[k] = PulseType::LOW;
          }
        }
      }
    }
    return {moduleMap, connectionMap};
  }

  Input inputStruct                 = ParseInput();
  ModuleMap_type& moduleMap         = inputStruct.moduleMap;
  ConnectionMap_type& connectionMap = inputStruct.connectionMap;

  std::unordered_map<PulseType, int> pulseCounters;

  void SendPulse(const std::string& moduleStr = "broadcaster",
                 const PulseType& pulseType = PulseType::LOW) {
    std::vector<std::pair<std::string, PulseType>> pulsesToSend;

    for (const auto& child : connectionMap.at(moduleStr)) {
      ++pulseCounters[pulseType];
      if (auto moduleMapIt = moduleMap.find(child); moduleMapIt != moduleMap.end()) {
        auto& childModule = moduleMapIt->second;
        if (childModule.moduleType == ModuleType::FLIPFLOP) {
          if (pulseType == PulseType::LOW) {
            const auto toSendPulseType = (childModule.state) ? PulseType::LOW : PulseType::HIGH;
            pulsesToSend.emplace_back(child, toSendPulseType);
            childModule.state = !childModule.state;
          }
        } else if (childModule.moduleType == ModuleType::CONJUNCTION) {
          childModule.pulseMap[moduleStr] = pulseType;
          const auto isLowPulse =
              std::all_of(childModule.pulseMap.begin(), childModule.pulseMap.end(),
                          [&](const auto& p) { return p.second == PulseType::HIGH; });
          const auto toSendPulseType = (isLowPulse) ? PulseType::LOW : PulseType::HIGH;
          pulsesToSend.emplace_back(child, toSendPulseType);
        }
      }
    }

    for (const auto& [child, toSendPulseType] : pulsesToSend) {
      SendPulse(child, toSendPulseType);
    }
  }

  std::string part_1() override {
    static constexpr auto numPresses = 1000;

    for (int i = 0; i < numPresses; ++i) {
      ++pulseCounters[PulseType::LOW];
      SendPulse();
    }

    return std::to_string(pulseCounters.at(PulseType::LOW) * pulseCounters.at(PulseType::HIGH));
  }

  std::string part_2() override {
    inputStruct = ParseInput();
    // to do tomorrow after sleep
    // Probably some sort of reverse-engineering/cycle detection of conjunction modules that feed
    // into final register
    return "";
  }
};

};  // namespace AOC2023
