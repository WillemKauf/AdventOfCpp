/////////////////
//// std
/////////////////
#include <optional>
#include <variant>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2016 {

struct day_25 : public Advent_type {
  static constexpr int year                         = 2016;
  static constexpr int date                         = 25;
  const std::vector<std::vector<std::string>> input = read_lines_vector_regex<std::string>(
      year, date, "(cpy) (\\w+) (\\w+)", "^(cpy) (-?[0-9]+) (\\w+)$", "^(jnz) (-?[0-9]+) (\\w+)$",
      "^(jnz) (\\w+) (-?[0-9]+)$", "(inc) (\\w+)", "(dec) (\\w+)", "(tgl) (\\w+)", "(out) (\\w+)");

  enum class InstructType {
    CPY,
    INC,
    DEC,
    JNZ,
    TGL,
    OUT
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

        std::variant<std::monostate, char, int> operandTwo;
        try {
          operandTwo = std::stoi(v[2]);
        } catch (...) {
          operandTwo = v[2][0];
        }

        instructs.emplace_back(InstructType::JNZ, operandOne, operandTwo);
      } else if (v[0] == "tgl") {
        instructs.emplace_back(InstructType::TGL, v[1][0], std::monostate{});
      } else if (v[0] == "out") {
        instructs.emplace_back(InstructType::OUT, v[1][0], std::monostate{});
      } else {
        throw std::runtime_error("Invalid type.");
      }
    }
    return instructs;
  }

  const std::vector<Instruct> instructInput = ParseInput();

  bool runInstructs(int aReg) const {
    std::unordered_map<char, int> registers = {{'a', aReg}, {'b', 0}, {'c', 0}, {'d', 0}};
    int instructPtr                         = 0;
    const auto inputSize                    = instructInput.size();
    auto instructs                          = instructInput;
    std::optional<int> prevValOpt           = {};
    int seqLength                           = 0;
    while (instructPtr < inputSize) {
      const auto& instruct = instructs[instructPtr];
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
          const auto& opTwo = instruct.operandTwo;
          int increment     = 1;
          if (std::holds_alternative<char>(opOne)) {
            if (registers.at(std::get<char>(opOne)) != 0) {
              if (std::holds_alternative<char>(opTwo)) {
                increment = registers.at(std::get<char>(opTwo));
              } else {
                increment = std::get<int>(opTwo);
              }
            }
          } else {
            if (std::get<int>(opOne) != 0) {
              if (std::holds_alternative<char>(opTwo)) {
                increment = registers.at(std::get<char>(opTwo));
              } else {
                increment = std::get<int>(opTwo);
              }
            }
          }
          instructPtr += increment;
          break;
        }
        case InstructType::TGL: {
          const auto& opOne = std::get<char>(instruct.operandOne);
          const auto& v     = registers.at(opOne);
          if (instructPtr + v < inputSize) {
            auto& tglInstruct = instructs[instructPtr + v];
            switch (tglInstruct.instructType) {
              case InstructType::CPY:
                tglInstruct.instructType = InstructType::JNZ;
                break;
              case InstructType::INC:
                tglInstruct.instructType = InstructType::DEC;
                break;
              case InstructType::DEC:
                tglInstruct.instructType = InstructType::INC;
                break;
              case InstructType::JNZ:
                tglInstruct.instructType = InstructType::CPY;
                break;
              case InstructType::TGL:
                tglInstruct.instructType = InstructType::INC;
                break;
            }
          }
          ++instructPtr;
          break;
        }
        case InstructType::OUT: {
          const auto& opOne = std::get<char>(instruct.operandOne);
          int currVal       = registers.at(opOne);
          if (prevValOpt.has_value()) {
            const auto& prevVal = prevValOpt.value();
            if ((prevVal == 0 && currVal == 1) || (prevVal == 1 && currVal == 0)) {
              if (seqLength++ == 30) {
                return true;
              }
            } else {
              return false;
            }
          }
          prevValOpt = currVal;
          ++instructPtr;
          break;
        }
      }
    }
    std::unreachable();
  }

  std::string part_1() override {
    int i = 0;
    for (; runInstructs(i) == false; ++i) {
    }
    return std::to_string(i);
  }

  std::string part_2() override { return DayTwentyFiveString; }
};

};  // namespace AOC2016
