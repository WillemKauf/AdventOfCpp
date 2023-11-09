/////////////////
//// std
/////////////////
#include <cstdint>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <variant>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

namespace AOC2015 {

struct day_07 : public Advent_type {
  static constexpr int year = 2015;
  static constexpr int date = 7;

  using Map_type = std::unordered_map<std::string, uint16_t>;
  Map_type wireMap;

  enum class Operation {
    kASSIGN,
    kAND,
    kNOT,
    kOR,
    kLSHIFT,
    kRSHIFT
  };

  struct Instruct {
    using Variant_type = std::variant<std::string, uint16_t>;
    int index;
    Operation operation;
    Variant_type lOperand;
    std::optional<Variant_type> rOperand;
    std::string dest;
  };

  const std::vector<Instruct> input = parse_input();

  Instruct MakeInstruct(int i, const std::vector<std::string>& instructVec) {
    Instruct::Variant_type lOperand;
    const auto& lOperandStr = (instructVec.size() == 3) ? instructVec[1] : instructVec[0];
    if (std::isdigit(lOperandStr[0])) {
      lOperand = static_cast<uint16_t>(std::stoi(lOperandStr));
    } else {
      lOperand = lOperandStr;
    }
    switch (instructVec.size()) {
      case 2: {
        const auto& dest = instructVec[1];
        return Instruct{i, Operation::kASSIGN, lOperand, std::nullopt, dest};
      }
      case 3: {
        const auto& dest = instructVec[2];
        return Instruct{i, Operation::kNOT, lOperand, std::nullopt, dest};
      }
      case 4: {
        std::string_view operationStr = instructVec[1];
        const auto& dest              = instructVec[3];
        Operation operation;
        if (operationStr == "AND") {
          operation = Operation::kAND;
        } else if (operationStr == "OR") {
          operation = Operation::kOR;
        } else if (operationStr == "LSHIFT") {
          operation = Operation::kLSHIFT;
        } else if (operationStr == "RSHIFT") {
          operation = Operation::kRSHIFT;
        }
        Instruct::Variant_type rOperand;
        const auto& rOperandStr = instructVec[2];
        if (std::isdigit(rOperandStr[0])) {
          rOperand = static_cast<uint16_t>(std::stoi(rOperandStr));
        } else {
          rOperand = rOperandStr;
        }

        return Instruct{i, operation, lOperand, rOperand, dest};
      }
      default:
        std::unreachable();
    }
    std::unreachable();
  }

  std::vector<Instruct> parse_input() {
    const std::string regexStrOne   = R"((\w+)\s+(AND|OR|LSHIFT|RSHIFT)\s+(\w+)\s+->\s+(\w+))";
    const std::string regexStrTwo   = R"((NOT)\s+(\w+)\s+->\s+(\w+))";
    const std::string regexStrThree = R"((\w+)\s+->\s+(\w+))";
    const auto rawInput =
        read_lines_vector_regex<std::string>(year, date, regexStrOne, regexStrTwo, regexStrThree);
    std::vector<Instruct> vec;
    vec.reserve(rawInput.size());
    for (int i = 0; const auto& instructVec : rawInput) {
      vec.push_back(std::move(MakeInstruct(i, instructVec)));
      ++i;
    }
    return vec;
  }

  template <typename T>
  bool assignBinaryOperation(const Instruct::Variant_type& lOperand,
                             const Instruct::Variant_type& rOperand, const std::string& dest,
                             T func) {
    if (std::holds_alternative<std::string>(lOperand) &&
        std::holds_alternative<std::string>(rOperand)) {
      auto lOperandIt = wireMap.find(std::get<std::string>(lOperand));
      auto rOperandIt = wireMap.find(std::get<std::string>(rOperand));
      if (lOperandIt == wireMap.end() || rOperandIt == wireMap.end()) {
        return false;
      }
      wireMap[dest] = func(lOperandIt->second, rOperandIt->second);
    } else if (std::holds_alternative<std::string>(lOperand) &&
               std::holds_alternative<uint16_t>(rOperand)) {
      auto lOperandIt = wireMap.find(std::get<std::string>(lOperand));
      if (lOperandIt == wireMap.end()) {
        return false;
      }
      auto& rOperandValue = std::get<uint16_t>(rOperand);
      wireMap[dest]       = func(lOperandIt->second, rOperandValue);
    } else if (std::holds_alternative<uint16_t>(lOperand) &&
               std::holds_alternative<std::string>(rOperand)) {
      auto& lOperandValue = std::get<uint16_t>(lOperand);
      auto rOperandIt     = wireMap.find(std::get<std::string>(rOperand));
      if (rOperandIt == wireMap.end()) {
        return false;
      }
      wireMap[dest] = func(lOperandValue, rOperandIt->second);
    } else if (std::holds_alternative<uint16_t>(lOperand) &&
               std::holds_alternative<uint16_t>(rOperand)) {
      auto& lOperandValue = std::get<uint16_t>(lOperand);
      auto& rOperandValue = std::get<uint16_t>(rOperand);
      wireMap[dest]       = func(lOperandValue, rOperandValue);
    }
    return true;
  }

  void simulateInstructions(bool partTwo = false) {
    std::unordered_set<int> seenSet;
    const auto inputSize = input.size();
    while (true) {
      if (seenSet.size() == inputSize) break;
      for (const auto& instruct : input) {
        if (seenSet.contains(instruct.index)) continue;
        switch (instruct.operation) {
          case (Operation::kASSIGN): {
            if (partTwo && instruct.dest == "b") {
              seenSet.insert(instruct.index);
              break;
            }
            if (std::holds_alternative<std::string>(instruct.lOperand)) {
              auto lOperandIt = wireMap.find(std::get<std::string>(instruct.lOperand));
              if (lOperandIt == wireMap.end()) break;
              wireMap[instruct.dest] = lOperandIt->second;
            } else {
              wireMap[instruct.dest] = std::get<uint16_t>(instruct.lOperand);
            }
            seenSet.insert(instruct.index);
            break;
          }
          case (Operation::kAND): {
            const auto& rOperand = instruct.rOperand.value();
            auto flag            = assignBinaryOperation(instruct.lOperand, rOperand, instruct.dest,
                                                         [](const auto& a, const auto& b) { return a & b; });
            if (flag) {
              seenSet.insert(instruct.index);
            }
            break;
          }
          case (Operation::kNOT): {
            auto lOperandIt = wireMap.find(std::get<std::string>(instruct.lOperand));
            if (lOperandIt == wireMap.end()) break;
            wireMap[instruct.dest] = ~(lOperandIt->second);
            seenSet.insert(instruct.index);
            break;
          }
          case (Operation::kOR): {
            const auto& rOperand = instruct.rOperand.value();
            auto flag            = assignBinaryOperation(instruct.lOperand, rOperand, instruct.dest,
                                                         [](const auto& a, const auto& b) { return a | b; });
            if (flag) {
              seenSet.insert(instruct.index);
            }
            break;
          }
          case (Operation::kLSHIFT): {
            const auto& rOperand = instruct.rOperand.value();
            auto flag            = assignBinaryOperation(instruct.lOperand, rOperand, instruct.dest,
                                                         [](const auto& a, const auto& b) { return a << b; });
            if (flag) {
              seenSet.insert(instruct.index);
            }
            break;
          }
          case (Operation::kRSHIFT): {
            const auto& rOperand = instruct.rOperand.value();
            auto flag            = assignBinaryOperation(instruct.lOperand, rOperand, instruct.dest,
                                                         [](const auto& a, const auto& b) { return a >> b; });
            if (flag) {
              seenSet.insert(instruct.index);
            }
            break;
          }
          default:
            std::unreachable();
        }
      }
    }
  }

  std::string part_1() override {
    simulateInstructions();
    return std::to_string(wireMap.at("a"));
  }

  std::string part_2() override {
    const auto aWireValue = wireMap.at("a");
    wireMap.clear();
    wireMap["b"] = aWireValue;
    simulateInstructions(true);
    return std::to_string(wireMap.at("a"));
  }
};

};  // namespace AOC2015
