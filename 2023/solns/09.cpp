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

struct day_09 : public Advent_type {
  static constexpr int year                 = 2023;
  static constexpr int date                 = 9;
  const std::vector<std::vector<int>> input = read_lines_vector_regex<int>(year, date, "(-?\\d+)");

  using Int_type    = int;
  using Vector_type = std::vector<Int_type>;

  template <bool partTwo = false>
  auto GetHistory(const Vector_type& history) {
    Vector_type edgeValues;
    if constexpr (partTwo) {
      edgeValues.push_back(history.front());
    } else {
      edgeValues.push_back(history.back());
    }
    Vector_type sequence = history;
    while (!std::all_of(sequence.begin(), sequence.end(), [](const auto& i) { return i == 0; })) {
      // Oh my god, why can't I opt out of std::adjacent_difference rewriting the first value with
      // the extrapolated first value of the range when all I want is the differences between values
      // this is horrible
      // Vector_type newSequence(sequence.size(), -1);
      // std::adjacent_difference(sequence.begin(), sequence.end(), newSequence.begin());
      // newSequence.erase(newSequence.begin());
      // And this way we avoid two initializations of vector values anyways, along with the extra
      // allocation space and call to .erase(). This STL function really sucks.
      Vector_type newSequence;
      newSequence.reserve(sequence.size() - 1);
      for (const auto& ab : std::views::slide(sequence, 2)) {
        newSequence.push_back(ab[1] - ab[0]);
      }

      sequence = std::move(newSequence);
      if constexpr (partTwo) {
        edgeValues.push_back(sequence.front());
      } else {
        edgeValues.push_back(sequence.back());
      }
    }
    if constexpr (partTwo) {
      const auto reverseView = std::views::reverse(edgeValues);
      return std::accumulate(reverseView.begin(), reverseView.end(), 0,
                             [](Int_type sm, Int_type edgeValue) { return edgeValue - sm; });
    } else {
      return Algorithm::Sum(edgeValues);
    }
  }

  std::string part_1() override {
    return std::to_string(
        std::accumulate(input.begin(), input.end(), 0,
                        [&](Int_type sm, const auto& v) { return sm + GetHistory(v); }));
  }

  std::string part_2() override {
    return std::to_string(
        std::accumulate(input.begin(), input.end(), 0,
                        [&](Int_type sm, const auto& v) { return sm + GetHistory<true>(v); }));
  }
};

};  // namespace AOC2023
