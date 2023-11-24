/////////////////
//// std
/////////////////

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2017 {

struct day_16 : public Advent_type {
  static constexpr int year = 2017;
  static constexpr int date = 16;
  const std::vector<std::string> input =
      Strings::Split(read_lines<std::string>(year, date)[0], ",");

  static constexpr int numPrograms = 16;

  enum class DanceType {
    SPIN,
    EXCHANGE,
    PARTNER
  };

  struct DanceMove {
    DanceMove(DanceType danceType, const std::string& a, const std::string& b)
        : danceType(danceType), a(a), b(b) {}
    DanceType danceType;
    std::string a;
    std::string b;
  };

  std::vector<DanceMove> ParseInput() const {
    std::vector<DanceMove> moves;
    for (const auto& v : input) {
      if (v[0] == 's') {
        moves.emplace_back(DanceType::SPIN, v.substr(1), "");
      } else if (v[0] == 'x') {
        const auto slashPos = v.find_first_of('/');
        moves.emplace_back(DanceType::EXCHANGE, v.substr(1, slashPos - 1), v.substr(slashPos + 1));
      } else if (v[0] == 'p') {
        const auto slashPos = v.find_first_of('/');
        moves.emplace_back(DanceType::PARTNER, v.substr(1, slashPos - 1), v.substr(slashPos + 1));
      } else {
        throw std::runtime_error("Invalid v[0]");
      }
    }
    return moves;
  }

  const std::vector<DanceMove> danceMoves = ParseInput();

  using ProgramMap_type  = std::unordered_map<std::string, int>;
  using PositionMap_type = std::map<int, std::string>;

  void PerformDance(ProgramMap_type& programMap, PositionMap_type& positionMap) const {
    for (const auto& danceMove : danceMoves) {
      switch (danceMove.danceType) {
        case DanceType::SPIN: {
          auto newProgramMap  = programMap;
          auto newPositionMap = positionMap;
          const auto numSpin  = std::stoi(danceMove.a);
          for (int i = 0; i < numPrograms - numSpin; ++i) {
            const auto c = positionMap.at(i);
            auto& pos    = newProgramMap.at(c);
            pos += numSpin;
            newPositionMap.at(pos) = c;
          }

          for (int i = 0; i < numSpin; ++i) {
            const auto c           = positionMap.at(numPrograms - numSpin + i);
            auto& pos              = newProgramMap.at(c);
            pos                    = i;
            newPositionMap.at(pos) = c;
          }

          programMap  = std::move(newProgramMap);
          positionMap = std::move(newPositionMap);

          break;
        }
        case DanceType::EXCHANGE: {
          const auto& positionA = std::stoi(danceMove.a);
          const auto& positionB = std::stoi(danceMove.b);
          auto& programA        = positionMap.at(positionA);
          auto& programB        = positionMap.at(positionB);
          std::swap(programA, programB);
          std::swap(programMap.at(programA), programMap.at(programB));
          break;
        }
        case DanceType::PARTNER: {
          const auto& programA = danceMove.a;
          const auto& programB = danceMove.b;
          auto& positionA      = programMap.at(programA);
          auto& positionB      = programMap.at(programB);
          std::swap(positionA, positionB);
          std::swap(positionMap.at(positionA), positionMap.at(positionB));
          break;
        }
      }
    }
  }

  std::string part_1() override {
    ProgramMap_type programMap;
    PositionMap_type positionMap;
    for (int i = 0; i < numPrograms; ++i) {
      const auto programStr = {static_cast<char>(i + 'a')};
      programMap.insert_or_assign(programStr, i);
      positionMap.insert_or_assign(i, programStr);
    }

    PerformDance(programMap, positionMap);

    return std::accumulate(
        positionMap.begin(), positionMap.end(), std::string{},
        [](const std::string& currStr, const auto& p) { return std::move(currStr) + p.second; });
  }

  std::string part_2() override {
    static constexpr int numDances = 1e9;
    ProgramMap_type programMap;
    PositionMap_type positionMap;
    for (int i = 0; i < numPrograms; ++i) {
      const auto programStr = {static_cast<char>(i + 'a')};
      programMap.insert_or_assign(programStr, i);
      positionMap.insert_or_assign(i, programStr);
    }

    std::unordered_map<std::string, int> seenHashes;
    for (int i = 0; i < numDances; ++i) {
      PerformDance(programMap, positionMap);
      const auto hashStr = std::accumulate(
          positionMap.begin(), positionMap.end(), std::string{},
          [](const std::string& currStr, const auto& p) { return std::move(currStr) + p.second; });
      if (auto seenHashIt = seenHashes.find(hashStr); seenHashIt != seenHashes.end()) {
        const auto prevSeen  = seenHashIt->second;
        const auto cycleSize = i - prevSeen;
        const auto nHash     = (numDances - (prevSeen + 1)) % cycleSize;
        return std::find_if(seenHashes.begin(), seenHashes.end(),
                            [&](const auto& p) { return p.second == nHash; })
            ->first;
      }
      seenHashes.insert_or_assign(hashStr, i);
    }
    std::unreachable();
  }
};

};  // namespace AOC2017
