/////////////////
//// std
/////////////////
#include <cmath>
#include <unordered_set>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2023 {

struct day_04 : public Advent_type {
  static constexpr int year            = 2023;
  static constexpr int date            = 4;
  const std::vector<std::string> input = read_lines<std::string>(year, date);

  using Set_type   = std::unordered_set<int>;
  using Pair_type  = std::pair<Set_type, Set_type>;
  using Cards_type = std::vector<Pair_type>;

  Cards_type ParseInput() const {
    Cards_type cards;
    cards.reserve(input.size());

    for (const auto& v : input) {
      const auto twoSides = Strings::Split(v, " | ");

      Set_type leftCard;
      const auto& left = twoSides[0];
      const auto leftP = left.find_first_of(':');
      for (const auto& num : Strings::Split(left.substr(leftP + 2), " ")) {
        if (!num.empty()) {
          leftCard.insert(std::stoi(num));
        }
      }

      Set_type rightCard;
      const auto& right = twoSides[1];
      for (const auto& num : Strings::Split(right, " ")) {
        if (!num.empty()) {
          rightCard.insert(std::stoi(num));
        }
      }
      cards.emplace_back(std::move(leftCard), std::move(rightCard));
    }

    return cards;
  }

  const Cards_type cards = ParseInput();

  std::string part_1() override {
    return std::to_string(
        std::accumulate(cards.begin(), cards.end(), 0, [](int ans, const auto& card) {
          const auto& leftCard   = card.first;
          const auto& rightCard  = card.second;
          const auto numMatching = Algorithm::SetIntersection(leftCard, rightCard).size();
          return ans + ((numMatching == 0) ? 0 : std::pow(2, numMatching - 1));
        }));
  }

  int Recurse(int p) const {
    static std::unordered_map<int, int> numCards;

    if (p >= cards.size()) {
      return 0;
    }

    if (auto numCardsIt = numCards.find(p); numCardsIt != numCards.end()) {
      return numCardsIt->second;
    }

    const auto& card       = cards[p];
    const auto& leftCard   = card.first;
    const auto& rightCard  = card.second;
    const auto numMatching = Algorithm::SetIntersection(leftCard, rightCard).size();

    int ans = numMatching;
    for (int i = 1; i <= numMatching; ++i) {
      ans += Recurse(p + i);
    }

    numCards.insert_or_assign(p, ans);
    return ans;
  }

  std::string part_2() override {
    const auto cardIndices = std::views::iota(0, static_cast<int>(cards.size()));
    return std::to_string(std::accumulate(cardIndices.begin(), cardIndices.end(), cards.size(),
                                          [&](int sm, const auto& i) { return sm + Recurse(i); }));
  }
};

};  // namespace AOC2023
