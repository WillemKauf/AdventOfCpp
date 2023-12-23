/////////////////
//// std
/////////////////
#include <algorithm>

/////////////////
//// local
/////////////////

#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2023 {

struct day_22 : public Advent_type {
  static constexpr int year            = 2023;
  static constexpr int date            = 22;
  const std::vector<std::string> input = read_lines<std::string>(year, date);

  using PosInt_type   = uint16_t;
  using Pos_type      = std::array<PosInt_type, 3>;
  using BrickTag_type = int;

  struct Brick {
    BrickTag_type c;
    Pos_type pOne;
    Pos_type pTwo;

    auto operator<=>(const Brick& other) const {
      return std::tie(pTwo[2], pTwo[1], pTwo[0]) <=>
             std::tie(other.pTwo[2], other.pTwo[1], other.pTwo[0]);
    }

    std::string ToString() const {
      return {c + ": " + std::to_string(pOne[0]) + "," + std::to_string(pOne[1]) + "," +
              std::to_string(pOne[2]) + "~" + std::to_string(pTwo[0]) + "," +
              std::to_string(pTwo[1]) + "," + std::to_string(pTwo[2])};
    }
  };

  bool IsCollision(const Brick& brickOne, const Brick& brickTwo) {
    auto collisionTest = [](const auto& brickA, const auto& brickB) {
      const auto xIntersect =
          (brickA.pOne[0] <= brickB.pTwo[0]) && (brickB.pTwo[0] <= brickA.pTwo[0]);
      const auto yIntersect =
          (brickB.pOne[1] <= brickA.pOne[1]) && (brickA.pOne[1] <= brickB.pTwo[1]);
      return xIntersect && yIntersect;
    };

    return collisionTest(brickOne, brickTwo) || collisionTest(brickTwo, brickOne);
  }

  using Bricks_type = std::vector<Brick>;

  Bricks_type ParseInput() const {
    Bricks_type bricks;
    BrickTag_type c = 0;
    for (const auto& v : input) {
      const auto LR = Strings::Split(v, "~");

      Pos_type pOne;
      for (const auto& [i, v] : std::views::enumerate(Strings::Split(LR[0], ","))) {
        pOne[i] = std::stoi(v);
      }

      Pos_type pTwo;
      for (const auto& [i, v] : std::views::enumerate(Strings::Split(LR[1], ","))) {
        pTwo[i] = std::stoi(v);
      }
      // Woah! c++!
      bricks.emplace_back(c++, pOne, pTwo);
    }
    std::sort(bricks.begin(), bricks.end());
    return bricks;
  }

  const Bricks_type inputBricks = ParseInput();

  using Map_type = std::unordered_map<BrickTag_type, std::unordered_set<BrickTag_type>>;
  Map_type supportedMap;  // Which blocks are supported by key.
  Map_type supportMap;    // Which blocks support key.

  std::string part_1() override {
    auto bricks      = inputBricks;
    auto& firstBrick = bricks[0];

    if (firstBrick.pOne[2] != 1) {
      //"Zero" out first brick.
      const auto zHeight = firstBrick.pTwo[2] - firstBrick.pOne[2];
      firstBrick.pOne[2] = 1;
      firstBrick.pTwo[2] = 1 + zHeight;
    }

    std::priority_queue<Brick> prevBricks;
    prevBricks.push(firstBrick);
    for (auto [index, brick] : std::views::enumerate(bricks)) {
      if (index == 0) {
        continue;
      }

      const auto zHeight = brick.pTwo[2] - brick.pOne[2];

      std::optional<int> zCollision = std::nullopt;
      // Test all bricks backwards from index to 0.
      auto prevBricksCpy = prevBricks;
      while (!prevBricksCpy.empty()) {
        const auto belowBrick = prevBricksCpy.top();
        prevBricksCpy.pop();
        if (IsCollision(brick, belowBrick)) {
          if (zCollision.has_value() && (belowBrick.pTwo[2] != zCollision.value())) {
            break;
          }

          if (!zCollision.has_value()) {
            zCollision    = belowBrick.pTwo[2];
            brick.pOne[2] = belowBrick.pTwo[2] + 1;
            brick.pTwo[2] = brick.pOne[2] + zHeight;
          }

          supportedMap[belowBrick.c].insert(brick.c);
          supportMap[brick.c].insert(belowBrick.c);
        }
      }
      if (!zCollision.has_value()) {
        brick.pOne[2] = 1;
        brick.pTwo[2] = brick.pOne[2] + zHeight;
      }
      prevBricks.push(brick);
    }

    int numToDisintegrate = 0;
    for (const auto& brick : bricks) {
      const auto c                = brick.c;
      const auto& supportedBricks = supportedMap[c];
      if (std::all_of(supportedBricks.begin(), supportedBricks.end(),
                      [&](const auto& cc) { return (supportMap[cc].size() > 1); })) {
        ++numToDisintegrate;
      }
    }
    return std::to_string(numToDisintegrate);
  }

  using DegreeMap_type = std::unordered_map<BrickTag_type, int>;

  int TopoSort(const BrickTag_type c, DegreeMap_type outDegrees) const {
    int ans = 0;
    std::queue<BrickTag_type> q;
    q.push(c);
    while (!q.empty()) {
      const auto curr = q.front();
      q.pop();
      const auto& supportedBlocks = supportedMap.at(curr);
      for (const auto& supportedBlock : supportedBlocks) {
        if (--outDegrees.at(supportedBlock) == 0) {
          q.push(supportedBlock);
          ++ans;
        }
      }
    }
    return ans;
  }

  std::string part_2() override {
    DegreeMap_type outDegrees;

    for (const auto& [k, v] : supportMap) {
      outDegrees[k] = v.size();
    }

    int ans = 0;
    for (const auto& brick : inputBricks) {
      ans += TopoSort(brick.c, outDegrees);
    }

    return std::to_string(ans);
  }
};

};  // namespace AOC2023
