/////////////////
//// std
/////////////////

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2016 {

struct day_20 : public Advent_type {
  static constexpr int year = 2016;
  static constexpr int date = 20;
  using Integer_type        = uint32_t;
  const std::vector<std::vector<Integer_type>> input =
      read_lines_vector_regex<Integer_type>(year, date, "(\\d+)-(\\d+)");

  std::vector<std::vector<uint32_t>> intervalPairs;

  std::string part_1() override {
    auto blackList = input;
    // Must sort by start time.
    std::sort(blackList.begin(), blackList.end(),
              [](const auto& a, const auto& b) { return a[0] < b[0]; });

    auto prevInterval = blackList[0];
    for (const auto& v : std::ranges::subrange(std::next(blackList.begin()), blackList.end())) {
      const auto& a  = prevInterval[0];
      const auto& b  = prevInterval[1];
      const auto& aa = v[0];
      const auto& bb = v[1];
      if (aa <= b) {
        prevInterval[1] = std::max(b, bb);
      } else {
        intervalPairs.push_back(prevInterval);
        prevInterval = v;
      }
    }

    intervalPairs.push_back(prevInterval);

    prevInterval = intervalPairs[0];
    for (const auto& v :
         std::ranges::subrange(std::next(intervalPairs.begin()), intervalPairs.end())) {
      const auto& a  = prevInterval[0];
      const auto& b  = prevInterval[1];
      const auto& aa = v[0];
      const auto& bb = v[1];
      if (aa > b + 1) {
        return std::to_string(b + 1);
      } else {
        prevInterval = v;
      }
    }

    std::unreachable();
  }

  std::string part_2() override {
    auto prevInterval = intervalPairs[0];
    return std::to_string(std::accumulate(std::next(intervalPairs.begin()), intervalPairs.end(), 0,
                                          [&prevInterval](Integer_type numIps, const auto& v) {
                                            const auto& a  = prevInterval[0];
                                            const auto& b  = prevInterval[1];
                                            const auto& aa = v[0];
                                            const auto& bb = v[1];
                                            numIps += (aa - b - 1);
                                            prevInterval = v;
                                            return numIps;
                                          }));
  }
};

};  // namespace AOC2016
