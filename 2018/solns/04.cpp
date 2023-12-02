/////////////////
//// std
/////////////////
#include <algorithm>
#include <chrono>
#include <ctime>
#include <limits>
#include <unordered_map>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2018 {

struct day_04 : public Advent_type {
  static constexpr int year                         = 2018;
  static constexpr int date                         = 4;
  const std::vector<std::vector<std::string>> input = read_lines_vector_regex<std::string>(
      year, date, "\\[(\\d+)-(\\d+)-(\\d+) (\\d+):(\\d+)\\] \?(.*)");

  using TimePoint_type = std::chrono::system_clock::time_point;
  using Minutes_type   = std::chrono::minutes;

  static constexpr Minutes_type minuteIt = Minutes_type{1};

  enum class EventType {
    BEGINS_SHIFT,
    FALLS_ASLEEP,
    WAKES_UP
  };

  enum class StateType {
    NONE,
    ON_SHIFT,
    ASLEEP
  };

  struct State {
    StateType stateType           = StateType::NONE;
    TimePoint_type sleepStartTime = {};
    int guardID                   = -1;
  };

  struct Record {
    TimePoint_type timePoint;
    EventType eventType;
    int guardID;

    auto operator<=>(const Record& other) const { return timePoint <=> other.timePoint; }
  };

  Record GetRecord(const std::vector<std::string>& rawInput) const {
    const auto& rYear = year;  // std::stoi(rawInput[0]); Cannot use a year before 1900 with
                               // std::tm, so we'll use the current year.
    const auto& rMonth  = std::stoi(rawInput[1]);
    const auto& rDay    = std::stoi(rawInput[2]);
    const auto& rHour   = std::stoi(rawInput[3]);
    const auto& rMinute = std::stoi(rawInput[4]);

    std::tm timeStruct       = {};
    timeStruct.tm_year       = rYear - 1900;
    timeStruct.tm_mon        = rMonth - 1;
    timeStruct.tm_mday       = rDay;
    timeStruct.tm_hour       = rHour;
    timeStruct.tm_min        = rMinute;
    std::time_t time         = std::mktime(&timeStruct);
    TimePoint_type timePoint = std::chrono::system_clock::from_time_t(time);

    int guardID = -1;
    EventType eventType;
    static const std::string guardStr       = "Guard";
    static const std::string wakeUpStr      = "wakes up";
    static const std::string fallsAsleepStr = "falls asleep";
    if (rawInput[5].substr(0, guardStr.size()) == guardStr) {
      static const auto subStrP = guardStr.size() + 2;
      guardID   = std::stoi(rawInput[5].substr(subStrP, rawInput[5].find_first_of(' ', subStrP)));
      eventType = EventType::BEGINS_SHIFT;
    } else if (rawInput[5] == wakeUpStr) {
      eventType = EventType::WAKES_UP;
    } else if (rawInput[5] == fallsAsleepStr) {
      eventType = EventType::FALLS_ASLEEP;
    } else {
      throw std::runtime_error("Invalid eventType");
    }
    return {timePoint, eventType, guardID};
  }

  std::vector<Record> ParseInput() const {
    std::vector<Record> records;
    for (const auto& v : input) {
      records.emplace_back(GetRecord(v));
    }
    std::sort(records.begin(), records.end());
    return records;
  }

  const std::vector<Record> inputRecords = ParseInput();

  std::unordered_map<int, int> guardSleepMap                              = {};
  std::unordered_map<int, std::unordered_map<int, int>> guardSleepTimeMap = {};

  std::string part_1() override {
    auto state = State{};
    for (const auto& record : inputRecords) {
      switch (record.eventType) {
        case EventType::BEGINS_SHIFT: {
          state.stateType = StateType::ON_SHIFT;
          state.guardID   = record.guardID;
          break;
        }
        case EventType::FALLS_ASLEEP: {
          state.stateType      = StateType::ASLEEP;
          state.sleepStartTime = record.timePoint;
          break;
        }
        case EventType::WAKES_UP: {
          state.stateType = StateType::ON_SHIFT;
          for (TimePoint_type currentTime = state.sleepStartTime; currentTime < record.timePoint;
               currentTime += minuteIt) {
            const auto currentMinute =
                std::chrono::time_point_cast<Minutes_type>(currentTime).time_since_epoch().count() %
                60;
            ++guardSleepMap[state.guardID];
            ++guardSleepTimeMap[state.guardID][currentMinute];
            state.sleepStartTime = {};
          }
          break;
        }
        default:
          throw std::runtime_error("Invalid eventType");
      }
    }

    const auto maxGuardID =
        std::max_element(guardSleepMap.begin(), guardSleepMap.end(),
                         [](const auto& a, const auto& b) { return a.second < b.second; })
            ->first;
    const auto maxMinute =
        std::max_element(guardSleepTimeMap.at(maxGuardID).begin(),
                         guardSleepTimeMap.at(maxGuardID).end(),
                         [](const auto& a, const auto& b) { return a.second < b.second; })
            ->first;

    return std::to_string(maxGuardID * maxMinute);
  }

  std::string part_2() override {
    int maxFreq    = std::numeric_limits<int>::min();
    int maxMinute  = {};
    int maxGuardID = {};
    for (const auto& [guardID, guardMap] : guardSleepTimeMap) {
      for (const auto& [minute, freq] : guardMap) {
        if (freq > maxFreq) {
          maxFreq    = freq;
          maxMinute  = minute;
          maxGuardID = guardID;
        }
      }
    }
    return std::to_string(maxGuardID * maxMinute);
  }
};

};  // namespace AOC2018
