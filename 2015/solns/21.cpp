/////////////////
//// std
/////////////////
#include <cmath>
#include <limits>
#include <stdexcept>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

struct day_21 : public Advent_type {
  static constexpr int year = 2015;
  static constexpr int date = 21;

  enum class ItemType {
    WEAPON,
    ARMOR,
    RING
  };

  struct Item {
    Item() = default;
    Item(const std::string& name, int cost, int damage, int armor)
        : name(name), cost(cost), damage(damage), armor(armor) {}

    std::string name;
    int cost;
    int damage;
    int armor;

    bool operator==(const Item& other) const {
      return std::tie(name, cost, damage, armor) ==
             std::tie(other.name, other.cost, other.damage, other.armor);
    }
  };

  struct Stats {
    Stats() = default;

    Stats(const Item& item) : hp(100), damage(item.damage), armor(item.armor) {}

    void operator+=(const Item& item) {
      damage += item.damage;
      armor += item.armor;
    }

    int hp;
    int damage;
    int armor;
  };

  using ShopMap_type = std::unordered_map<ItemType, std::vector<Item>>;

  struct Input {
    Stats boss;
    ShopMap_type shop;
  };

  Input parseInput() const {
    Stats boss;
    {
      const std::vector<std::vector<std::string>> bossInput =
          read_lines_vector_regex<std::string>(year, date, "(\\w+): (\\d+)");
      for (const auto& v : bossInput) {
        const auto& statName = v[0];
        const auto statValue = std::stoi(v[1]);
        if (statName == "Points") {
          boss.hp = statValue;
        } else if (statName == "Damage") {
          boss.damage = statValue;
        } else if (statName == "Armor") {
          boss.armor = statValue;
        } else {
          throw std::runtime_error("Invalid statName");
        }
      }
    }
    ShopMap_type shopMap;
    {
      auto shopFile = get_input_file(year, date);
      shopFile.insert(shopFile.find_last_of('.'), std::string("_shop"));
      const std::vector<std::vector<std::string>> shopInput =
          read_lines_vector_regex<std::string>(shopFile, "(\\w+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)",
                                               "(\\w+):\\s+(\\w+)\\s+(\\w+)\\s+(\\w+)");
      ItemType itemType;
      for (const auto& v : shopInput) {
        if (v.empty()) continue;
        if (!std::isdigit(v[1][0])) {
          if (v[0] == "Weapons") {
            itemType = ItemType::WEAPON;
          } else if (v[0] == "Armor") {
            itemType = ItemType::ARMOR;
          } else if (v[0] == "Rings") {
            itemType = ItemType::RING;
          } else {
            throw std::runtime_error("Invalid v[0]");
          }
          continue;
        }
        auto& mp           = shopMap[itemType];
        const auto& name   = v[0];
        const auto& cost   = std::stoi(v[1]);
        const auto& damage = std::stoi(v[2]);
        const auto& armor  = std::stoi(v[3]);
        mp.emplace_back(name, cost, damage, armor);
      }
      shopMap[ItemType::ARMOR].emplace_back("None", 0, 0, 0);
      shopMap[ItemType::RING].emplace_back("None", 0, 0, 0);
    }
    return {boss, shopMap};
  }

  const Input input           = parseInput();
  const Stats& bossStats      = input.boss;
  const ShopMap_type& shopMap = input.shop;

  auto simulateFight(Stats player) const {
    auto turn = 1;
    auto boss = bossStats;
    while (true) {
      auto& attacker = turn ? player : boss;
      auto& defender = turn ? boss : player;
      defender.hp -= std::max(1, attacker.damage - defender.armor);
      if (defender.hp <= 0) {
        return static_cast<bool>(turn);
      }
      turn ^= 1;
    }
    std::unreachable();
  }

  template <bool wantToWin = true>
  auto simulateFights(int money) {
    for (const auto& weapon : shopMap.at(ItemType::WEAPON)) {
      for (const auto& armor : shopMap.at(ItemType::ARMOR)) {
        const auto& ringMap = shopMap.at(ItemType::RING);
        for (auto ringItOne = ringMap.begin(); ringItOne != ringMap.end(); ++ringItOne) {
          for (auto ringItTwo = ringItOne; ringItTwo != ringMap.end(); ++ringItTwo) {
            if (ringItOne->name != "None" && *ringItOne == *ringItTwo) continue;
            Stats player(weapon);
            player += armor;
            player += *ringItOne;
            player += *ringItTwo;
            const auto cost = weapon.cost + armor.cost + ringItOne->cost + ringItTwo->cost;

            if constexpr (wantToWin) {
              if (cost > money) {
                continue;
              }
            } else {
              if (cost < money) {
                continue;
              }
            }
            const auto result = simulateFight(player);
            if constexpr (wantToWin) {
              if (result) {
                return true;
              }
            } else {
              if (!result) {
                return true;
              }
            }
          }
        }
      }
    }
    return false;
  }

  std::string part_1() override {
    int l = 0;
    int r = std::accumulate(shopMap.begin(), shopMap.end(), 0, [](int sm, const auto& pair) {
      return sm + std::accumulate(pair.second.begin(), pair.second.end(), 0,
                                  [](int vecSm, const auto& item) { return vecSm + item.cost; });
    });

    while (l < r) {
      int m = (l + r) / 2;
      if (simulateFights(m)) {
        r = m;
      } else {
        l = m + 1;
      }
    }
    return std::to_string(l);
  }

  std::string part_2() override {
    int l = 0;
    int r = std::accumulate(shopMap.begin(), shopMap.end(), 0, [](int sm, const auto& pair) {
      return sm + std::accumulate(pair.second.begin(), pair.second.end(), 0,
                                  [](int vecSm, const auto& item) { return vecSm + item.cost; });
    });

    while (l < r) {
      int m = (l + r) / 2;
      if (simulateFights<false>(m)) {
        l = m;
      } else {
        r = m - 1;
      }
    }
    return std::to_string(l);
  }
};
