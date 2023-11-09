/////////////////
//// std
/////////////////
#include <algorithm>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

namespace AOC2016 {

struct day_10 : public Advent_type {
  static constexpr int year                         = 2016;
  static constexpr int date                         = 10;
  const std::vector<std::vector<std::string>> input = read_lines_vector_regex<std::string>(
      year, date, "value (\\d+) goes to bot (\\d+)",
      "bot (\\d+) gives low to (\\w+) (\\d+) and high to (\\w+) (\\d+)");
  static constexpr int specialLowChip  = 17;
  static constexpr int specialHighChip = 61;

  enum class TargetType {
    BOT,
    OUTPUT
  };

  struct Target {
    TargetType targetType;
    int targetNum;
  };

  enum class InstructionType {
    VALUE,
    BOTGIVE
  };

  struct Instruction {
    Instruction(InstructionType instructType, Target targetA, std::optional<int> source,
                std::optional<Target> targetB, std::optional<int> value)
        : instructType(instructType),
          targetA(targetA),
          source(source),
          targetB(targetB),
          value(value) {}
    InstructionType instructType;
    Target targetA;
    std::optional<int> source;
    std::optional<Target> targetB;
    std::optional<int> value;
    bool complete = false;
  };

  std::vector<Instruction> ParseInput() {
    std::vector<Instruction> instructs;
    instructs.reserve(input.size());
    const auto getTargetType = [&](std::string_view str) {
      if (str == "bot") {
        return TargetType::BOT;
      } else {
        return TargetType::OUTPUT;
      }
    };
    for (const auto& v : input) {
      if (v.size() == 2) {
        const auto value     = std::stoi(v[0]);
        const auto targetNum = std::stoi(v[1]);
        instructs.emplace_back(InstructionType::VALUE, Target{TargetType::BOT, targetNum},
                               std::nullopt, std::nullopt, value);
      } else if (v.size() == 5) {
        const auto source      = std::stoi(v[0]);
        const auto targetTypeA = getTargetType(v[1]);
        const auto targetNumA  = std::stoi(v[2]);
        const auto targetTypeB = getTargetType(v[3]);
        const auto targetNumB  = std::stoi(v[4]);
        instructs.emplace_back(InstructionType::BOTGIVE, Target{targetTypeA, targetNumA}, source,
                               Target{targetTypeB, targetNumB}, std::nullopt);
      }
    }
    return instructs;
  }

  std::vector<Instruction> instructs = ParseInput();

  struct Bot {
    std::optional<int> lowChip;
    std::optional<int> highChip;

    void GetChip(int chip) {
      if (lowChip.has_value()) {
        highChip = chip;
        if (lowChip.value() > chip) {
          std::swap(lowChip, highChip);
        }
      } else {
        lowChip = chip;
      }
    }

    bool CanGive() const { return lowChip.has_value() && highChip.has_value(); }

    void Give() {
      lowChip  = std::nullopt;
      highChip = std::nullopt;
    }
  };

  std::unordered_map<int, int> outputMap;

  std::string part_1() override {
    int specialBotNum = {};
    std::unordered_map<int, Bot> botMap;
    while (true) {
      if (std::all_of(instructs.begin(), instructs.end(),
                      [](const auto& instruct) { return instruct.complete; })) {
        break;
      }
      for (auto& instruct : instructs) {
        if (instruct.complete) {
          continue;
        }
        switch (instruct.instructType) {
          case InstructionType::VALUE: {
            const auto& target = instruct.targetA.targetNum;
            const auto& value  = instruct.value.value();
            botMap[target].GetChip(value);
            instruct.complete = true;
            break;
          }
          case InstructionType::BOTGIVE: {
            const auto& botNum = instruct.source.value();
            auto& bot          = botMap[botNum];
            if (bot.CanGive()) {
              const auto& lowTargetType  = instruct.targetA.targetType;
              const auto& lowTargetNum   = instruct.targetA.targetNum;
              const auto& highTargetType = instruct.targetB.value().targetType;
              const auto& highTargetNum  = instruct.targetB.value().targetNum;
              if (bot.lowChip.value() == specialLowChip &&
                  bot.highChip.value() == specialHighChip) {
                specialBotNum = botNum;
              }
              switch (lowTargetType) {
                case TargetType::BOT: {
                  botMap[lowTargetNum].GetChip(bot.lowChip.value());
                  break;
                }
                case TargetType::OUTPUT: {
                  outputMap[lowTargetNum] = bot.lowChip.value();
                  break;
                }
              }
              switch (highTargetType) {
                case TargetType::BOT: {
                  botMap[highTargetNum].GetChip(bot.highChip.value());
                  break;
                }
                case TargetType::OUTPUT: {
                  outputMap[highTargetNum] = bot.highChip.value();
                  break;
                }
              }
              bot.Give();
              instruct.complete = true;
            }
            break;
          }
        }
      }
    }
    return std::to_string(specialBotNum);
  }

  std::string part_2() override {
    return std::to_string(outputMap.at(0) * outputMap.at(1) * outputMap.at(2));
  }
};

};  // namespace AOC2016
