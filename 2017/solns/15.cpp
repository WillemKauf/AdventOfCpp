/////////////////
//// std
/////////////////
#include <cstdint>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2017 {

struct day_15 : public Advent_type {
  static constexpr int year                 = 2017;
  static constexpr int date                 = 15;
  const std::vector<std::vector<int>> input = read_lines_vector_regex<int>(year, date, "(\\d+)");

  using Int_type = uint64_t;

  static constexpr Int_type modFactor = 2147483647;

  struct Generator {
    Generator(Int_type startingValue, Int_type mulFactor)
        : startingValue(startingValue), value(startingValue), mulFactor(mulFactor) {}

    void Reset() { value = startingValue; }

    Int_type GenerateNextValue() { return value = (value * mulFactor) % modFactor; }

    Int_type startingValue;
    Int_type value;
    Int_type mulFactor;
  };

  struct Generators {
    Generator genA;
    Generator genB;
  };

  Generators ParseInput() const {
    static constexpr Int_type mulFactorA = 16807;
    static constexpr Int_type mulFactorB = 48271;
    return {{static_cast<Int_type>(input[0][0]), mulFactorA},
            {static_cast<Int_type>(input[1][0]), mulFactorB}};
  };

  Generators generators = ParseInput();
  Generator& genA       = generators.genA;
  Generator& genB       = generators.genB;

  template <bool partTwo = false>
  std::pair<Int_type, Int_type> GenerateNextValues() const {
    if constexpr (partTwo) {
      static constexpr Int_type multipleA = 4;
      static constexpr Int_type multipleB = 8;
      const auto generateValue            = [](auto& gen, const auto& multiple) {
        Int_type v = gen.GenerateNextValue();
        while (v % multiple != 0) {
          v = gen.GenerateNextValue();
        }
        return v;
      };
      return {generateValue(genA, multipleA), generateValue(genB, multipleB)};
    } else {
      return {genA.GenerateNextValue(), genB.GenerateNextValue()};
    }
  }

  template <bool partTwo = false>
  int GetCount(int numPairs) {
    if constexpr (partTwo) {
      genA.Reset();
      genB.Reset();
    }

    static constexpr int numBits = 16;
    using Bitset_type            = std::bitset<numBits>;

    int count = 0;
    for (int i = 0; i < numPairs; ++i) {
      const auto [a, b] = GenerateNextValues<partTwo>();
      const auto bitA   = Bitset_type(a);
      const auto bitB   = Bitset_type(b);
      count += (bitA == bitB);
    }

    return count;
  }

  std::string part_1() override {
    static constexpr int numPairs = 4e7;
    return std::to_string(GetCount(numPairs));
  }

  std::string part_2() override {
    static constexpr int numPairs = 5e6;
    return std::to_string(GetCount<true>(numPairs));
  }
};

};  // namespace AOC2017
