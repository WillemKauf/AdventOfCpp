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

template <bool partTwo>
static const auto cardMappings = {};

template <>
const auto cardMappings<false> = DataStructure::MakeOrdering(
    std::vector<char>{'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'});

template <>
const auto cardMappings<true> = DataStructure::MakeOrdering(
    std::vector<char>{'J', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'Q', 'K', 'A'});

struct day_07 : public Advent_type {
  static constexpr int year = 2023;
  static constexpr int date = 7;
  const std::vector<std::vector<std::string>> input =
      read_lines_vector_regex<std::string>(year, date, "(\\w+) (\\w+)");

  enum class HandType {
    HIGH_CARD,
    PAIR,
    TWO_PAIR,
    THREE_KIND,
    FULL_HOUSE,
    FOUR_KIND,
    FIVE_KIND
  };

  class Player {
  public:
    Player(const std::string& hand, int bid)
        : hand(hand), bid(bid), handType(InitHandType()), handTypePartTwo(InitHandType<true>()) {}
    std::string hand;
    int bid;
    HandType handType;
    HandType handTypePartTwo;

    template <bool partTwo = false>
    HandType GetHandType() const {
      if constexpr (partTwo) {
        return handTypePartTwo;
      } else {
        return handType;
      }
    }

  private:
    template <bool partTwo = false>
    HandType InitHandType() const {
      auto handCpy = hand;
      if constexpr (partTwo) {
        const auto handCounter =
            DataStructure::Counter(handCpy).template get_sorted_by_freq<true>();
        const auto mostCommonCard = handCounter.begin();
        const auto replacementCard =
            (mostCommonCard->first == 'J')
                ? ((mostCommonCard->second == 5) ? 'A' : std::next(mostCommonCard)->first)
                : mostCommonCard->first;
        std::replace(handCpy.begin(), handCpy.end(), 'J', replacementCard);
      }
      const auto handCounter = DataStructure::Counter(handCpy).template get_sorted_by_freq<true>();
      switch (handCounter.size()) {
        case 1:
          return HandType::FIVE_KIND;
        case 2:
          return (handCounter.begin()->second == 4) ? HandType::FOUR_KIND : HandType::FULL_HOUSE;
        case 3:
          return (handCounter.begin()->second == 3) ? HandType::THREE_KIND : HandType::TWO_PAIR;
        case 4:
          return HandType::PAIR;
        case 5:
          return HandType::HIGH_CARD;
        default:
          throw std::runtime_error("Invalid handCounter.size()");
      }
      std::unreachable();
    }
  };

  using Players_type = std::vector<Player>;

  Players_type ParseInput() const {
    Players_type players;
    players.reserve(input.size());
    for (const auto& v : input) {
      players.emplace_back(v[0], std::stoi(v[1]));
    }
    return players;
  }

  const Players_type inputPlayers = ParseInput();

  template <bool partTwo = false>
  struct PlayerCmp {
    static auto operator()(const Player& a, const Player& b) {
      const auto aType = a.GetHandType<partTwo>();
      const auto bType = b.GetHandType<partTwo>();
      if (aType != bType) {
        return aType < bType;
      } else {
        for (const auto& [cA, cB] : std::views::zip(a.hand, b.hand)) {
          const auto& vA = cardMappings<partTwo>.at(cA);
          const auto& vB = cardMappings<partTwo>.at(cB);
          if (vA != vB) {
            return vA < vB;
          }
        }
      }
      std::unreachable();
    }
  };

  template <bool partTwo = false>
  int GetWinnings() const {
    auto players = inputPlayers;

    std::sort(players.begin(), players.end(), PlayerCmp<partTwo>{});
    const auto& enumerateView = std::views::enumerate(players);
    return std::accumulate(enumerateView.begin(), enumerateView.end(), 0,
                           [](int sm, const auto& enumView) {
                             return sm + (std::get<0>(enumView) + 1) * std::get<1>(enumView).bid;
                           });
  }

  std::string part_1() override { return std::to_string(GetWinnings()); }

  std::string part_2() override { return std::to_string(GetWinnings<true>()); }
};

};  // namespace AOC2023
