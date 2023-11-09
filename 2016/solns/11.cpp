/////////////////
//// std
/////////////////
#include <algorithm>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

namespace AOC2016 {

struct day_11 : public Advent_type {
  static constexpr int year = 2016;
  static constexpr int date = 11;
  const std::vector<std::vector<std::string>> input =
      read_lines_vector_regex<std::string>(year, date, R"(([\w-]+) (microchip|generator))");

  enum class ObjectType {
    MC,
    GEN
  };

  using Bit_type = uint16_t;

  struct Object {
    std::string objectName;
    ObjectType objectType;
    uint8_t floor;
    Bit_type bitCode;

    auto operator<=>(const Object& other) const {
      return std::tie(bitCode) <=> std::tie(other.bitCode);
    }

    std::string toString() {
      if (objectType == ObjectType::GEN) {
        return objectName.substr(0, 1) + "G";
      } else {
        return objectName.substr(0, 1) + "M";
      }
    }
  };

  struct State {
    std::vector<Object> objects;
    int numObjects;
    int numSteps      = 0;
    uint8_t currFloor = 1;

    static constexpr Bit_type numBits     = 16;
    static constexpr Bit_type halfNumBits = numBits / 2;
    static constexpr uint8_t numFloors    = 4;

    bool isValid() const {
      if (currFloor < 1 || currFloor > numFloors) {
        return false;
      }
      for (uint8_t floor = 1; floor <= numFloors; ++floor) {
        auto floorView = std::views::filter(
            objects, [&floor](const auto& object) { return object.floor == floor; });
        std::bitset<numBits> floorBit = {};
        bool isGeneratorPresent       = false;

        for (auto obj : floorView) {
          floorBit |= obj.bitCode;
          if (obj.objectType == ObjectType::GEN) {
            isGeneratorPresent = true;
          }
        }

        for (uint8_t i = 0; i < halfNumBits; ++i) {
          if (floorBit.test(i) && !floorBit.test(i + 8) && isGeneratorPresent) {
            return false;
          }
        }
      }
      return true;
    }

    void Print() const {
      for (uint8_t floor = numFloors; floor >= 1; --floor) {
        std::cout << "F" << int(floor) << " ";
        if (currFloor == floor) {
          std::cout << "E ";
        }
        auto floorView = std::views::filter(
            objects, [&floor](const auto& object) { return object.floor == floor; });
        for (auto obj : floorView) {
          std::cout << obj.toString() << " ";
        }
        std::cout << '\n';
      }
      std::cout << '\n';
    }

    uint64_t getHash() const {
      uint64_t hash = 0;
      for (uint8_t floor = 1; floor <= numFloors; ++floor) {
        Bit_type floorHash = {};
        auto floorView     = std::views::filter(
            objects, [&floor](const auto& object) { return object.floor == floor; });
        for (auto obj : floorView) {
          floorHash |= obj.bitCode;
        }
        hash |= (static_cast<uint64_t>(floorHash) << ((floor - 1) * numBits));
      }
      hash |= (static_cast<uint64_t>(currFloor) << 61);
      return hash;
    }

    bool isDone() const {
      std::bitset<48> lowerFloorHash(getHash());
      return lowerFloorHash.none();
    }

    std::vector<State> getNextStates() const {
      std::vector<State> states;
      for (const auto& dir : {1, -1}) {
        const auto nextFloor = currFloor + dir;
        if (nextFloor < 1 || nextFloor > numFloors) {
          continue;
        }
        // Prune states, do not consider moves downwards if there are no items below currFloor.
        if (dir == -1) {
          auto hash       = getHash();
          bool shouldMove = false;
          for (uint8_t floor = 1; floor < currFloor; ++floor) {
            std::bitset<numBits> bitHash(hash);
            if (bitHash.any()) {
              shouldMove = true;
              break;
            }
            hash >> numBits;
          }
          if (!shouldMove) {
            continue;
          }
        }
        for (int i = 0; i < objects.size(); ++i) {
          if (objects[i].floor != currFloor) continue;
          auto moveOneState = *this;
          auto& objectA     = moveOneState.objects[i];
          objectA.floor     = nextFloor;
          ++moveOneState.numSteps;
          moveOneState.currFloor = nextFloor;
          if (moveOneState.isValid()) {
            states.push_back(moveOneState);
          }
          for (int j = i + 1; j < objects.size(); ++j) {
            if (objects[j].floor != currFloor) continue;
            auto moveTwoState = moveOneState;
            auto& objectB     = moveTwoState.objects[j];
            objectB.floor     = nextFloor;
            if (moveTwoState.isValid()) {
              states.push_back(moveTwoState);
            }
          }
        }
      }
      return states;
    }

    auto operator<=>(const State& other) const {
      const auto hash      = getHash();
      const auto otherHash = other.getHash();
      return std::tie(otherHash, numSteps) <=> std::tie(hash, other.numSteps);
    };
  };

  State ParseInput() const {
    std::vector<Object> objects;
    std::unordered_map<std::string, Bit_type> objectBitMap;
    for (auto [floorIndex, vec] : std::views::enumerate(input)) {
      for (const auto& object : std::views::chunk(vec, 2)) {
        const auto& objectTypeStr = object[1];
        const auto isGen          = (objectTypeStr == "generator");
        const auto objectNameStr =
            isGen ? object[0] : object[0].substr(0, object[0].find_first_of('-'));
        const auto objectType = isGen ? ObjectType::GEN : ObjectType::MC;
        if (!objectBitMap.contains(objectNameStr)) {
          objectBitMap.insert_or_assign(objectNameStr, objectBitMap.size());
        }
        const auto objShift =
            isGen ? (8 + objectBitMap.at(objectNameStr)) : objectBitMap.at(objectNameStr);
        objects.emplace_back(objectNameStr, objectType, floorIndex + 1, (1 << objShift));
      }
    }
    return {objects, static_cast<int>(objectBitMap.size())};
  }

  const State startingState = ParseInput();

  int pathFind(const State& originalState) const {
    int minSteps = std::numeric_limits<int>::max();
    std::priority_queue<State, std::vector<State>, std::greater<State>> stateQueue;
    stateQueue.push(originalState);
    std::unordered_map<uint64_t, int> seenHashMap;
    long long seenStates = 0;
    while (!stateQueue.empty()) {
      auto currState = std::move(stateQueue.top());
      ++seenStates;
      stateQueue.pop();
      if (currState.numSteps >= minSteps) {
        continue;
      }

      if (currState.isDone()) {
        minSteps = std::min(minSteps, currState.numSteps);
        break;
      }

      const auto currHash   = currState.getHash();
      const auto seenHashIt = seenHashMap.find(currHash);
      if (seenHashIt != seenHashMap.end() && seenHashIt->second <= currState.numSteps) {
        continue;
      }

      seenHashMap.insert_or_assign(currHash, currState.numSteps);

      for (const auto& nextState : currState.getNextStates()) {
        stateQueue.push(nextState);
      }
    }

    return minSteps;
  }

  std::string part_1() override { return std::to_string(pathFind(startingState)); }

  std::string part_2() override {
    auto originalState = startingState;
    originalState.objects.emplace_back("elerium", ObjectType::GEN, 1,
                                       (1 << (8 + startingState.numObjects)));
    originalState.objects.emplace_back("elerium", ObjectType::MC, 1,
                                       (1 << startingState.numObjects));
    originalState.objects.emplace_back("dilithium", ObjectType::GEN, 1,
                                       (1 << (8 + (startingState.numObjects + 1))));
    originalState.objects.emplace_back("dilithium", ObjectType::MC, 1,
                                       (1 << (startingState.numObjects + 1)));
    return std::to_string(pathFind(originalState));
  }
};

};  // namespace AOC2016
