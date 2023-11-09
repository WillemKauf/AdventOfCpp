/////////////////
//// std
/////////////////
#include <stdexcept>
#include <unordered_map>
#include <variant>
#include <vector>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

namespace AOC2016 {

struct day_12 : public Advent_type {
  static constexpr int year                         = 2016;
  static constexpr int date                         = 12;
  const std::vector<std::vector<std::string>> input = read_lines_vector_regex<std::string>(
      year, date, "(cpy) (\\w+) (\\w+)", "^(jnz) (\\w+) (-?[0-9]+)$", "(inc) (\\w+)",
      "(dec) (\\w+)");

  enum class InstructType {
    CPY,
    INC,
    DEC,
    JNZ
  };

  struct Instruct {
    Instruct(InstructType instructType, const std::variant<char, int>& operandOne,
             const std::variant<std::monostate, char, int>& operandTwo)
        : instructType(instructType), operandOne(operandOne), operandTwo(operandTwo) {}
    InstructType instructType;
    std::variant<char, int> operandOne;
    std::variant<std::monostate, char, int> operandTwo;
  };

  std::vector<Instruct> ParseInput() const {
    std::vector<Instruct> instructs;
    instructs.reserve(input.size());
    for (const auto& v : input) {
      if (v[0] == "cpy") {
        std::variant<char, int> operandOne;
        try {
          operandOne = std::stoi(v[1]);
        } catch (...) {
          operandOne = v[1][0];
        }
        instructs.emplace_back(InstructType::CPY, operandOne, v[2][0]);
      } else if (v[0] == "inc") {
        instructs.emplace_back(InstructType::INC, v[1][0], std::monostate{});
      } else if (v[0] == "dec") {
        instructs.emplace_back(InstructType::DEC, v[1][0], std::monostate{});
      } else if (v[0] == "jnz") {
        std::variant<char, int> operandOne;
        try {
          operandOne = std::stoi(v[1]);
        } catch (...) {
          operandOne = v[1][0];
        }
        instructs.emplace_back(InstructType::JNZ, operandOne, std::stoi(v[2]));
      } else {
        throw std::runtime_error("Invalid type.");
      }
    }
    return instructs;
  }

  const std::vector<Instruct> instructInput = ParseInput();

  template <int cReg = 0>
  int runInstructs() const {
    std::unordered_map<char, int> registers = {{'a', 0}, {'b', 0}, {'c', cReg}, {'d', 0}};
    int instructPtr                         = 0;
    const auto inputSize                    = instructInput.size();
    while (instructPtr < inputSize) {
      const auto& instruct = instructInput[instructPtr];
      switch (instruct.instructType) {
        case InstructType::CPY: {
          const auto& opOne = instruct.operandOne;
          const auto& opTwo = std::get<char>(instruct.operandTwo);
          if (std::holds_alternative<char>(opOne)) {
            registers.at(opTwo) = registers.at(std::get<char>(opOne));
          } else {
            registers.at(opTwo) = std::get<int>(opOne);
          }
          ++instructPtr;
          break;
        }
        case InstructType::INC: {
          const auto& opOne = std::get<char>(instruct.operandOne);
          ++registers.at(opOne);
          ++instructPtr;
          break;
        }
        case InstructType::DEC: {
          const auto& opOne = std::get<char>(instruct.operandOne);
          --registers.at(opOne);
          ++instructPtr;
          break;
        }
        case InstructType::JNZ: {
          const auto& opOne = instruct.operandOne;
          const auto& opTwo = std::get<int>(instruct.operandTwo);
          int increment     = 1;
          if (std::holds_alternative<char>(opOne)) {
            if (registers.at(std::get<char>(opOne)) != 0) {
              increment = opTwo;
            }
          } else {
            if (std::get<int>(opOne) != 0) {
              increment = opTwo;
            }
          }
          instructPtr += increment;
          break;
        }
      }
    }
    return registers.at('a');
  }

  std::string part_1() override { return std::to_string(runInstructs()); }

  std::string part_2() override { return std::to_string(runInstructs<1>()); }
};

};  // namespace AOC2016
