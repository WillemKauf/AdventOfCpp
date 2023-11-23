/////////////////
//// std
/////////////////

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2017 {

struct day_08 : public Advent_type {
  static constexpr int year                         = 2017;
  static constexpr int date                         = 8;
  const std::vector<std::vector<std::string>> input = read_lines_vector_regex<std::string>(
      year, date, "(\\w+) (inc|dec) (-?\\d+) if (\\w+) ([><=!]+) (-?\\d+)");

  enum class OperationType {
    INC,
    DEC
  };

  enum class ComparisionType {
    GT,
    LT,
    GE,
    LE,
    EQ,
    NEQ
  };

  struct Instruct {
    Instruct(const std::string& reg, OperationType opType, int opAmount, const std::string& cmpReg,
             ComparisionType cmpType, int cmpValue)
        : reg(reg),
          opType(opType),
          opAmount(opAmount),
          cmpReg(cmpReg),
          cmpType(cmpType),
          cmpValue(cmpValue) {}

    std::string reg;
    OperationType opType;
    int opAmount;
    std::string cmpReg;
    ComparisionType cmpType;
    int cmpValue;
  };

  std::vector<Instruct> ParseInput() const {
    std::vector<Instruct> instructs;
    for (const auto& v : input) {
      const auto& reg            = v[0];
      const OperationType opType = (v[1] == "inc") ? OperationType::INC : OperationType::DEC;
      const auto opAmount        = std::stoi(v[2]);
      const auto& cmpReg         = v[3];
      const auto& cmpStr         = v[4];
      ComparisionType cmpType;
      if (cmpStr == ">") {
        cmpType = ComparisionType::GT;
      } else if (cmpStr == "<") {
        cmpType = ComparisionType::LT;
      } else if (cmpStr == ">=") {
        cmpType = ComparisionType::GE;
      } else if (cmpStr == "<=") {
        cmpType = ComparisionType::LE;
      } else if (cmpStr == "==") {
        cmpType = ComparisionType::EQ;
      } else if (cmpStr == "!=") {
        cmpType = ComparisionType::NEQ;
      }
      const auto cmpValue = std::stoi(v[5]);
      instructs.emplace_back(reg, opType, opAmount, cmpReg, cmpType, cmpValue);
    }
    return instructs;
  }

  const std::vector<Instruct> inputInstructs = ParseInput();

  // Part two variable.
  int maxRegisterValue = std::numeric_limits<int>::min();

  std::string part_1() override {
    std::unordered_map<std::string, int> registers;
    const std::unordered_map<ComparisionType, bool (*)(int, int)> cmpFunctionMap = {
        {ComparisionType::GT, [](int a, int b) { return a > b; }},
        {ComparisionType::LT, [](int a, int b) { return a < b; }},
        {ComparisionType::GE, [](int a, int b) { return a >= b; }},
        {ComparisionType::LE, [](int a, int b) { return a <= b; }},
        {ComparisionType::EQ, [](int a, int b) { return a == b; }},
        {ComparisionType::NEQ, [](int a, int b) { return a != b; }}};

    const std::unordered_map<OperationType, void (*)(int&, int)> opFunctionMap = {
        {OperationType::INC, [](int& v, int a) { v += a; }},
        {OperationType::DEC, [](int& v, int a) { v -= a; }}};
    for (const auto& instruct : inputInstructs) {
      const auto& cmpFunc = cmpFunctionMap.at(instruct.cmpType);
      if (cmpFunc(registers[instruct.cmpReg], instruct.cmpValue)) {
        const auto& opFunc = opFunctionMap.at(instruct.opType);
        opFunc(registers[instruct.reg], instruct.opAmount);
        maxRegisterValue = std::max(maxRegisterValue, registers[instruct.reg]);
      }
    }
    return std::to_string(
        std::max_element(registers.begin(), registers.end(), [](const auto& a, const auto& b) {
          return a.second < b.second;
        })->second);
  }

  std::string part_2() override { return std::to_string(maxRegisterValue); }
};

};  // namespace AOC2017
