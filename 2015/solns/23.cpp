/////////////////
//// std
/////////////////
#include <limits>
#include <stdexcept>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

namespace AOC2015 {

struct day_23 : public Advent_type {
  static constexpr int year = 2015;
  static constexpr int date = 23;

  enum class InstructType {
    hlf,
    tpl,
    inc,
    jmp,
    jie,
    jio
  };

  struct Instruct {
    Instruct(InstructType instructType, std::optional<std::string> reg, std::optional<int> offset)
        : instructType(instructType), reg(reg), offset(offset) {}
    InstructType instructType;
    std::optional<std::string> reg;
    std::optional<int> offset;
  };

  std::vector<Instruct> parseInput() const {
    const std::vector<std::vector<std::string>> rawInput = read_lines_vector_regex<std::string>(
        year, date, "(\\w+) ([+-]\\d+)", "(\\w+) (\\w+), ([+-]\\d+)", "(\\w+) (\\w+)");
    std::vector<Instruct> vec;
    vec.reserve(rawInput.size());
    for (const auto& v : rawInput) {
      InstructType instructType;
      std::optional<std::string> reg = std::nullopt;
      std::optional<int> offset      = std::nullopt;
      if (v.size() == 2) {
        const auto& instruct = v[0];
        const auto isJmp     = instruct == "jmp";

        if (isJmp) {
          offset = std::stoi(v[1]);
        } else {
          reg = v[1];
        }

        if (instruct == "hlf") {
          instructType = InstructType::hlf;
        } else if (instruct == "tpl") {
          instructType = InstructType::tpl;
        } else if (instruct == "jmp") {
          instructType = InstructType::jmp;
        } else if (instruct == "inc") {
          instructType = InstructType::inc;
        }
      } else if (v.size() == 3) {
        const auto& instruct = v[0];
        reg                  = v[1];
        offset               = std::stoi(v[2]);
        if (instruct == "jie") {
          instructType = InstructType::jie;
        } else if (instruct == "jio") {
          instructType = InstructType::jio;
        }
      } else {
        throw std::runtime_error("Invalid v.size()");
      }
      vec.emplace_back(instructType, reg, offset);
    }
    return vec;
  }

  const std::vector<Instruct> input = parseInput();
  const int inputSize               = input.size();

  int simulate(int aValue) const {
    int instructPtr                                = 0;
    std::unordered_map<std::string, int> registers = {{"a", aValue}, {"b", 0}};
    while (instructPtr < inputSize) {
      const auto& instruct = input[instructPtr];
      switch (instruct.instructType) {
        case (InstructType::hlf): {
          const auto& reg = instruct.reg.value();
          registers[reg] /= 2;
          ++instructPtr;
          break;
        }
        case (InstructType::tpl): {
          const auto& reg = instruct.reg.value();
          registers[reg] *= 3;
          ++instructPtr;
          break;
        }
        case (InstructType::inc): {
          const auto& reg = instruct.reg.value();
          ++registers[reg];
          ++instructPtr;
          break;
        }
        case (InstructType::jmp): {
          const auto& offset = instruct.offset.value();
          instructPtr += offset;
          break;
        }
        case (InstructType::jie): {
          const auto& reg    = instruct.reg.value();
          const auto& offset = instruct.offset.value();
          instructPtr += (registers[reg] % 2 == 0) ? offset : 1;
          break;
        }
        case (InstructType::jio): {
          const auto& reg    = instruct.reg.value();
          const auto& offset = instruct.offset.value();
          instructPtr += (registers[reg] == 1) ? offset : 1;
          break;
        }
        default:
          throw std::runtime_error("Invalid instructType");
      }
    }
    return registers.at("b");
  }

  std::string part_1() override { return std::to_string(simulate(0)); }

  std::string part_2() override { return std::to_string(simulate(1)); }
};

};  // namespace AOC2015
