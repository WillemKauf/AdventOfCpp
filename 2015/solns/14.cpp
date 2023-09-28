/////////////////
//// std
/////////////////
#include <limits>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

struct day_14 : public Advent_type {
  static constexpr int year = 2015;
  static constexpr int date = 14;

  struct Reindeer {
    Reindeer(const std::string& name, int speed, int moveTime, int restTime)
        : name(name), speed(speed), moveTime(moveTime), restTime(restTime) {}
    std::string name;
    int speed;
    int moveTime;
    int restTime;
  };

  auto getDistanceAtTime(int t, const Reindeer& reindeer) {
    const auto loopTime          = reindeer.moveTime + reindeer.restTime;
    const auto numLoops          = t / loopTime;
    const auto remainder         = t % loopTime;
    const auto moveTimeRemainder = std::min(reindeer.moveTime, remainder);
    const auto distance =
        numLoops * reindeer.moveTime * reindeer.speed + moveTimeRemainder * reindeer.speed;
    return distance;
  }

  std::vector<Reindeer> parseInput() const {
    const std::vector<std::vector<std::string>> rawInput =
        read_lines_vector_regex<std::string>(year, date,
                                             "([A-Za-z]+) can fly (\\d+) km/s for (\\d+) seconds, "
                                             "but then must rest for (\\d+) seconds\\.");
    std::vector<Reindeer> vec;
    vec.reserve(rawInput.size());
    for (const auto& v : rawInput) {
      vec.emplace_back(v[0], std::stoi(v[1]), std::stoi(v[2]), std::stoi(v[3]));
    }
    return vec;
  }

  const std::vector<Reindeer> input = parseInput();

  std::string part_1() override {
    static constexpr auto totalTime = 2503;
    int max                         = std::numeric_limits<int>::min();
    return std::to_string(
        std::accumulate(input.begin(), input.end(), std::numeric_limits<int>::min(),
                        [&](int currMax, const Reindeer& reindeer) {
                          return std::max(currMax, getDistanceAtTime(totalTime, reindeer));
                        }));
  }

  std::string part_2() override {
    static constexpr auto totalTime = 2503;
    std::unordered_map<std::string, int> scoreMap;
    for (int t = 1; t < totalTime; ++t) {
      std::vector<std::string> currLeaders;
      int leaderDistance = std::numeric_limits<int>::min();
      for (const auto& reindeer : input) {
        const auto distance = getDistanceAtTime(t, reindeer);
        if (distance > leaderDistance) {
          leaderDistance = distance;
          currLeaders    = {reindeer.name};
        } else if (distance == leaderDistance) {
          currLeaders.push_back(reindeer.name);
        }
      }
      for (const auto& leader : currLeaders) {
        ++scoreMap[leader];
      }
    }

    return std::to_string(
        std::max_element(scoreMap.begin(), scoreMap.end(), [](const auto& a, const auto& b) {
          return a.second < b.second;
        })->second);
  }
};
