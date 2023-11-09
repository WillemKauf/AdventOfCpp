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

namespace AOC2015 {

struct day_22 : public Advent_type {
  static constexpr int year = 2015;
  static constexpr int date = 22;

  // enum class SpellType {
  //   MAGICMISSILE,
  //   DRAIN,
  //   SHIELD,
  //   POISON,
  //   RECHARGE
  // };
  //
  // enum class Effect {
  //   SHIELD,
  //   POISON,
  //   RECHARGE
  // };
  //
  // using StateMap = std::unordered_map<std::string, int>
  //
  // struct MagicMissile {
  //   static std::string name = "MagicMissile";
  //   static int manaCost     = 53;
  //
  //   template <typename S, typename T>
  //   static void cast(S&, T& target) {
  //     target.hp -= 4;
  //   }
  // };
  //
  // struct Drain {
  //   static std::string name = "Drain";
  //   static int manaCost     = 73;
  //
  //   template <typename S, typename T>
  //   static void cast(S& self, T& target) {
  //     self.hp += 2;
  //     target.hp -= 2;
  //   }
  // };
  //
  // struct Shield {
  //   static std::string name = "Shield";
  //   static int manaCost     = 113;
  //   static int numTurns = 7;
  //
  //   template <typename S, typename T>
  //   static void cast(S& self, T&) {
  //     self.set_state(Effect::SHIELD, numTurns);
  //   }
  //
  //   template <typename T>
  //   static void effect(T& target) {
  //     target.armor += 7;
  //   }
  //
  //   template <typename T>
  //   static void end(T& target) {
  //     target.armor -= 7;
  //   }
  // };
  //
  // struct Poison {
  //   static std::string name = "Posion";
  //   static int manaCost     = 173;
  //   static int numTurns = 6;
  //
  //   template <typename S, typename T>
  //   static void cast(S&, T& target) {
  //     target.set_state(Effect::POISON, numTurns);
  //   }
  //
  //   template <typename T>
  //   static void effect(T& target) {
  //     target.hp -= 3;
  //   }
  //
  //   template <typename T>
  //   static void end(T&) {}
  // };
  //
  // struct Recharge {
  //   static std::string name = "Recharge";
  //   static int manaCost     = 229;
  //   static int numTurns = 5;
  //
  //   template <typename S, typename T>
  //   static void cast(S& self, T&) {
  //     self.set_state(Effect::RECHARGE, numTurns);
  //   }
  //
  //   template <typename T>
  //   static void effect(T& target) {
  //     target.mana += 101;
  //   }
  //
  //   template <typename T>
  //   static void end(T&) {}
  // };
  //
  // template<typename T>
  // void process_effect(Effect effect, int turnsLeft, T& obj){
  //   switch(effect){
  //     case Effect::SHIELD:
  //       if(turnsLeft == Shield::numTurns){
  //         Shield::effect(obj);
  //       }
  //       break;
  //     case Effect::POISON:
  //       Poison::effect(obj);
  //       break;
  //     case Effect::RECHARGE:
  //       RECHARGE::effect(obj);
  //       break;
  //   }
  // }
  //
  // struct Boss {
  //   int hp;
  //   int damage;
  //   int armor;
  //   StateMap state;
  //
  //   void set_state(Effect effect, int nTurns){
  //     state.insert_or_assign(effect, nTurns);
  //   }
  //
  //
  //   void process_state(){
  //     for(auto& [effect, turnsLeft] : state){
  //       if(turnsLeft == 0){
  //         continue;
  //       }
  //       process_effect(effect, turnsLeft--, *this);
  //     }
  //   }
  // };
  //
  //
  //
  //
  // Boss parseInput() const {
  //   Boss boss;
  //   const std::vector<std::vector<std::string>> bossInput =
  //       read_lines_vector_regex<std::string>(year, date, "(\\w+): (\\d+)");
  //   for (const auto& v : bossInput) {
  //     const auto& statName = v[0];
  //     const auto statValue = std::stoi(v[1]);
  //     if (statName == "Points") {
  //       boss.hp = statValue;
  //     } else if (statName == "Damage") {
  //       boss.damage = statValue;
  //     } else {
  //       throw std::runtime_error("Invalid statName");
  //     }
  //   }
  //
  //   return boss;
  // }
  //
  // const Boss bossStats = parseInput();
  //
  // auto simulateFight(Player player) const {
  //   auto turn = 1;
  //   auto boss = bossStats;
  //   while (true) {
  //   }
  //   std::unreachable();
  // }

  std::string part_1() override { return "Unimplemented"; }

  std::string part_2() override { return "Unimplemented"; }
};

};  // namespace AOC2015
