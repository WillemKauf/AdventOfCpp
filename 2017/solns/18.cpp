/////////////////
//// std
/////////////////
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <variant>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2017 {

struct day_18 : public Advent_type {
  static constexpr int year            = 2017;
  static constexpr int date            = 18;
  const std::vector<std::string> input = read_lines<std::string>(year, date);

  using Int_type = uint64_t;

  enum class InstructType {
    SND,
    SET,
    ADD,
    MUL,
    MOD,
    RCV,
    JGZ
  };

  struct Instruct {
    using Variant_type = std::variant<std::monostate, Int_type, char>;
    Instruct(InstructType instructType, const Variant_type& x, const Variant_type& y)
        : instructType(instructType), x(x), y(y) {}
    InstructType instructType;
    Variant_type x;
    Variant_type y;
  };

  std::vector<Instruct> parseInput() const {
    std::vector<Instruct> instructs;
    instructs.reserve(input.size());
    const auto getOperand = [](const auto& val) {
      Instruct::Variant_type op;
      try {
        op = static_cast<Int_type>(std::stoul(val));
      } catch (...) {
        op = val[0];
      }
      return op;
    };
    for (const auto& s : input) {
      const auto v = Strings::Split(s, " ");
      const auto x = getOperand(v[1]);
      const auto y = (v.size() == 3) ? getOperand(v[2]) : std::monostate{};
      if (v[0] == "snd") {
        instructs.emplace_back(InstructType::SND, x, y);
      } else if (v[0] == "set") {
        instructs.emplace_back(InstructType::SET, x, y);
      } else if (v[0] == "add") {
        instructs.emplace_back(InstructType::ADD, x, y);
      } else if (v[0] == "mul") {
        instructs.emplace_back(InstructType::MUL, x, y);
      } else if (v[0] == "mod") {
        instructs.emplace_back(InstructType::MOD, x, y);
      } else if (v[0] == "rcv") {
        instructs.emplace_back(InstructType::RCV, x, y);
      } else if (v[0] == "jgz") {
        instructs.emplace_back(InstructType::JGZ, x, y);
      } else {
        throw std::runtime_error("Invalid type.");
      }
    }

    return instructs;
  }

  const std::vector<Instruct> instructInput = parseInput();

  std::string part_1() override {
    std::unordered_map<char, Int_type> registers;
    Int_type lastSound   = {};
    const auto inputSize = instructInput.size();
    for (int p = 0; p < inputSize;) {
      const auto& instruct = instructInput[p];
      switch (instruct.instructType) {
        case InstructType::SND: {
          lastSound = registers[std::get<char>(instruct.x)];
          ++p;
          break;
        }
        case InstructType::SET: {
          const auto x  = std::get<char>(instruct.x);
          const auto& y = instruct.y;
          if (std::holds_alternative<char>(y)) {
            registers[x] = registers[std::get<char>(y)];
          } else {
            registers[x] = std::get<Int_type>(y);
          }
          ++p;
          break;
        }
        case InstructType::ADD: {
          const auto x  = std::get<char>(instruct.x);
          const auto& y = instruct.y;
          if (std::holds_alternative<char>(y)) {
            registers[x] += registers[std::get<char>(y)];
          } else {
            registers[x] += std::get<Int_type>(y);
          }
          ++p;
          break;
        }
        case InstructType::MUL: {
          const auto x  = std::get<char>(instruct.x);
          const auto& y = instruct.y;
          if (std::holds_alternative<char>(y)) {
            registers[x] *= registers[std::get<char>(y)];
          } else {
            registers[x] *= std::get<Int_type>(y);
          }
          ++p;
          break;
        }
        case InstructType::MOD: {
          const auto x  = std::get<char>(instruct.x);
          const auto& y = instruct.y;
          if (std::holds_alternative<char>(y)) {
            registers[x] %= registers[std::get<char>(y)];
          } else {
            registers[x] %= std::get<Int_type>(y);
          }
          ++p;
          break;
        }
        case InstructType::RCV: {
          const auto x = std::get<char>(instruct.x);
          if (registers[x] != 0) {
            return std::to_string(lastSound);
          }
          ++p;
          break;
        }
        case InstructType::JGZ: {
          const auto& x = instruct.x;
          const auto& y = instruct.y;
          int offset    = 1;
          if (std::holds_alternative<char>(y)) {
            if (std::holds_alternative<char>(x)) {
              if (registers[std::get<char>(x)] > 0) {
                offset = registers[std::get<char>(y)];
              }
            } else {
              if (std::get<Int_type>(x) > 0) {
                offset = registers[std::get<char>(y)];
              }
            }
          } else {
            if (std::holds_alternative<char>(x)) {
              if (registers[std::get<char>(x)] > 0) {
                offset = std::get<Int_type>(y);
              }
            } else {
              if (std::get<Int_type>(x) > 0) {
                offset = std::get<Int_type>(y);
              }
            }
          }
          p += offset;
          break;
        }
      }
    }
    std::unreachable();
  }

  class Program {
  public:
    Program(Int_type PID, const std::vector<Instruct>& instructInput)
        : PID(PID), instructInput(instructInput) {
      registers['p'] = PID;
    }

    void SetOtherProgram(Program* program) { otherProgram = program; }

    void RecvValue(Int_type value) {
      {
        std::unique_lock uniqueLock(mutex);
        q.push(value);
      }
      cv.notify_one();
    }

    void SendValue(Int_type value) { otherProgram->RecvValue(value); }

    int GetSendCount() const { return sendCount; }

    void Run() {
      const auto inputSize = instructInput.size();
      while (!breakFlag && p < inputSize) {
        const auto& instruct = instructInput[p];
        switch (instruct.instructType) {
          case InstructType::SND: {
            SendValue(registers[std::get<char>(instruct.x)]);
            ++p;
            ++sendCount;
            break;
          }
          case InstructType::SET: {
            const auto x  = std::get<char>(instruct.x);
            const auto& y = instruct.y;
            if (std::holds_alternative<char>(y)) {
              registers[x] = registers[std::get<char>(y)];
            } else {
              registers[x] = std::get<Int_type>(y);
            }
            ++p;
            break;
          }
          case InstructType::ADD: {
            const auto x  = std::get<char>(instruct.x);
            const auto& y = instruct.y;
            if (std::holds_alternative<char>(y)) {
              registers[x] += registers[std::get<char>(y)];
            } else {
              registers[x] += std::get<Int_type>(y);
            }
            ++p;
            break;
          }
          case InstructType::MUL: {
            const auto x  = std::get<char>(instruct.x);
            const auto& y = instruct.y;
            if (std::holds_alternative<char>(y)) {
              registers[x] *= registers[std::get<char>(y)];
            } else {
              registers[x] *= std::get<Int_type>(y);
            }
            ++p;
            break;
          }
          case InstructType::MOD: {
            const auto x  = std::get<char>(instruct.x);
            const auto& y = instruct.y;
            if (std::holds_alternative<char>(y)) {
              registers[x] %= registers[std::get<char>(y)];
            } else {
              registers[x] %= std::get<Int_type>(y);
            }
            ++p;
            break;
          }
          case InstructType::RCV: {
            std::unique_lock uniqueLock(mutex);
            using namespace std::chrono_literals;
            if (cv.wait_for(uniqueLock, 100ms, [&] { return !q.empty(); })) {
              registers[std::get<char>(instruct.x)] = q.front();
              q.pop();
              ++p;
            } else {
              breakFlag = true;
            }
            break;
          }
          case InstructType::JGZ: {
            const auto& x   = instruct.x;
            const auto& y   = instruct.y;
            Int_type offset = 1;
            if (std::holds_alternative<char>(y)) {
              if (std::holds_alternative<char>(x)) {
                if (registers[std::get<char>(x)] > 0) {
                  offset = registers[std::get<char>(y)];
                }
              } else {
                if (std::get<Int_type>(x) > 0) {
                  offset = registers[std::get<char>(y)];
                }
              }
            } else {
              if (std::holds_alternative<char>(x)) {
                if (registers[std::get<char>(x)] > 0) {
                  offset = std::get<Int_type>(y);
                }
              } else {
                if (std::get<Int_type>(x) > 0) {
                  offset = std::get<Int_type>(y);
                }
              }
            }
            p += offset;
            break;
          }
        }
      }
    }

  private:
    int PID               = {};
    int sendCount         = 0;
    int p                 = 0;
    Program* otherProgram = nullptr;
    std::unordered_map<char, Int_type> registers;
    const std::vector<Instruct> instructInput;
    std::queue<Int_type> q = {};
    std::condition_variable cv;
    std::mutex mutex;
    bool breakFlag = false;
  };

  std::string part_2() override {
    auto p0 = Program(0, instructInput);
    auto p1 = Program(1, instructInput);

    p0.SetOtherProgram(&p1);
    p1.SetOtherProgram(&p0);

    {
      auto t0 = std::jthread(&Program::Run, &p0);
      auto t1 = std::jthread(&Program::Run, &p1);
    }  //.join()

    return std::to_string(p1.GetSendCount());
  }
};

};  // namespace AOC2017
