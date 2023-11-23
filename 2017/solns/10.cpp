/////////////////
//// std
/////////////////
#include <numeric>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2017 {

struct day_10 : public Advent_type {
  static constexpr int year = 2017;
  static constexpr int date = 10;
  const std::vector<std::string> input =
      read_lines_vector_regex<std::string>(year, date, "(\\d+)")[0];
  static constexpr int size = 255 + 1;

  template <typename Numbers_T, typename Lengths_T>
  void RunHash(Numbers_T& numbers, const Lengths_T& lengths, int& p, int& skipSize) const {
    for (const auto& length : lengths) {
      Algorithm::CyclicReverse(numbers, p, p + length - 1);
      p = (p + length + skipSize++) % size;
    }
  }

  std::string part_1() override {
    std::array<int, size> numbers;
    std::iota(numbers.begin(), numbers.end(), 0);
    std::vector<int> lengths;
    for (const auto& v : input) {
      lengths.push_back(std::stoi(v));
    }
    int p        = 0;
    int skipSize = 0;
    RunHash(numbers, lengths, p, skipSize);
    return std::to_string(numbers[0] * numbers[1]);
  }

  std::string part_2() override {
    static constexpr int numRounds           = 64;
    static const std::vector<int> endLengths = {17, 31, 73, 47, 23};

    std::array<int, size> numbers;
    std::iota(numbers.begin(), numbers.end(), 0);

    const auto hashFunc = [](const auto& numbers) {
      std::vector<int> hashVec;
      for (const auto& number : numbers) {
        for (const auto& digit : number) {
          hashVec.push_back(static_cast<int>(digit));
        }
        hashVec.push_back(static_cast<int>(','));
      }
      hashVec.pop_back();
      return hashVec;
    };

    std::vector<int> lengths = hashFunc(input);
    lengths.insert(lengths.end(), endLengths.begin(), endLengths.end());

    int p        = 0;
    int skipSize = 0;
    for (int i = 0; i < numRounds; ++i) {
      RunHash(numbers, lengths, p, skipSize);
    }

    static constexpr auto blockSize = 16;
    std::vector<int> denseHash;
    denseHash.reserve(16);
    for (auto block : std::views::chunk(numbers, blockSize)) {
      auto outputNum = std::accumulate(block.begin(), block.end(), 0,
                                       [](int curr, const auto& v) { return curr ^ v; });
      denseHash.push_back(outputNum);
    }
    std::stringstream knotHash;
    for (const auto& num : denseHash) {
      knotHash << std::hex << num;
    }
    return knotHash.str();
  }
};

};  // namespace AOC2017
