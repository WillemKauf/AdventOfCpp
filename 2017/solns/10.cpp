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

  template <typename Container_T>
  static std::string RunKnotHash(const Container_T& inputContainer) {
    static constexpr int numRounds           = 64;
    static const std::vector<int> endLengths = {17, 31, 73, 47, 23};

    std::array<int, size> numbers;
    std::iota(numbers.begin(), numbers.end(), 0);

    const auto hashFunc = [](const auto& toHash) {
      std::vector<int> hashVec;
      for (const auto& v : toHash) {
        hashVec.push_back(static_cast<const int>(v));
      }
      return hashVec;
    };

    std::vector<int> lengths = hashFunc(inputContainer);
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
    return Algorithm::ToHex(denseHash);
  }

  template <typename Numbers_T, typename Lengths_T>
  static void RunHash(Numbers_T& numbers, const Lengths_T& lengths, int& p, int& skipSize) {
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
    std::vector<char> charVector;
    for (const auto& s : input) {
      for (const auto& c : s) {
        charVector.push_back(c);
      }
      charVector.push_back(',');
    }
    charVector.pop_back();
    return RunKnotHash(charVector);
  }
};

};  // namespace AOC2017
