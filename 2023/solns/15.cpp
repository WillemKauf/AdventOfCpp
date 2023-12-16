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

struct day_15 : public Advent_type {
  static constexpr int year = 2023;
  static constexpr int date = 15;
  const std::string input   = read_lines<std::string>(year, date)[0];

  using Hash_type = uint8_t;

  enum class OperationType {
    EQUALS,
    MINUS
  };

  struct Operation {
    Operation(const std::string& str, const std::string& label, OperationType operationType,
              int value = -1)
        : str(str), label(label), operationType(operationType), value(value) {}
    std::string str;
    std::string label;
    OperationType operationType;
    int value;
  };

  std::vector<Operation> ParseInput() const {
    std::vector<Operation> operations;

    for (const auto& opStr : Strings::Split(input, ",")) {
      if (opStr.back() == '-') {
        operations.emplace_back(opStr, opStr.substr(0, opStr.size() - 1), OperationType::MINUS);
      } else {
        const auto labelAndValue = Strings::Split(opStr, "=");
        operations.emplace_back(opStr, labelAndValue[0], OperationType::EQUALS,
                                std::stoi(labelAndValue[1]));
      }
    }
    return operations;
  }

  const std::vector<Operation> operations = ParseInput();

  static Hash_type Hash(std::string_view s) {
    Hash_type val = {};
    for (const auto& c : s) {
      val = ((val + static_cast<Hash_type>(c)) * 17) % 256;
    }
    return val;
  }

  std::string part_1() override {
    return std::to_string(std::accumulate(operations.begin(), operations.end(), 0,
                                          [](int sm, const auto& s) { return sm + Hash(s.str); }));
  }

  std::string part_2() override {
    struct KeyValue {
      KeyValue(std::string_view key, int value) : key(key), value(value) {}
      std::string_view key;
      int value;
    };

    std::unordered_map<Hash_type, std::vector<KeyValue>> boxMap;
    std::unordered_map<Hash_type, std::unordered_set<std::string_view>> boxLabelMap;
    for (const auto& op : operations) {
      const auto hash = Hash(op.label);
      auto& box       = boxMap[hash];
      auto& labels    = boxLabelMap[hash];
      switch (op.operationType) {
        case (OperationType::EQUALS): {
          if (labels.contains(op.label)) {
            for (auto& kv : box) {
              if (kv.key == op.label) {
                kv.value = op.value;
                break;
              }
            }
          } else {
            box.emplace_back(op.label, op.value);
            labels.insert(op.label);
          }
          break;
        }
        case (OperationType::MINUS): {
          if (labels.contains(op.label)) {
            box.erase(std::find_if(box.begin(), box.end(),
                                   [&](const auto& kv) { return kv.key == op.label; }));
            labels.erase(op.label);
          }
          break;
        }
        default:
          throw std::runtime_error("Invalid OperationType");
      }
    }
    return std::to_string(
        std::accumulate(boxMap.begin(), boxMap.end(), 0, [](int sm, const auto& p) {
          const auto& [boxNumber, box] = p;
          for (const auto& [slotNumber, kv] : std::views::enumerate(box)) {
            sm += (boxNumber + 1) * (slotNumber + 1) * kv.value;
          }
          return sm;
        }));
  }
};

};  // namespace AOC2023
