/////////////////
//// nlohmann json
/////////////////
#include <nlohmann/json.hpp>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

namespace AOC2015 {

struct day_12 : public Advent_type {
  using json                = nlohmann::json;
  static constexpr int year = 2015;
  static constexpr int date = 12;

  json parseInput() const {
    auto inputFileStr = get_input_file(year, date);
    std::ifstream inputFile(inputFileStr);
    return json::parse(inputFile);
  }

  const json input = parseInput();

  template <bool ignoreRed, typename T>
  int jsonRecurse(const T& j) {
    int sm = 0;
    if (j.is_array()) {
      for (const auto& e : j) {
        sm += jsonRecurse<ignoreRed>(e);
      }
    } else if (j.is_object()) {
      for (const auto& [k, v] : j.items()) {
        if constexpr (ignoreRed) {
          if (v.is_string()) {
            const auto valueStr = v.template get<std::string>();
            if (valueStr == "red") {
              return 0;
            }
          }
        }
        sm += jsonRecurse<ignoreRed>(v);
      }
    } else if (j.is_number()) {
      sm += j.template get<int>();
    }
    return sm;
  }

  std::string part_1() override { return std::to_string(jsonRecurse<false>(input)); }

  std::string part_2() override { return std::to_string(jsonRecurse<true>(input)); }
};

};  // namespace AOC2015
