#ifndef COMMON_H
#define COMMON_H

/////////////////
//// md5 lib
/////////////////
#include <openssl/md5.h>

/////////////////
//// std
/////////////////
#include <algorithm>
#include <deque>
#include <iomanip>
#include <limits>
#include <sstream>

/////////////////
//// local
/////////////////
#include "config.h"

namespace Grid {

enum class GridDirection {
  WithDiagonals,
  WithoutDiagonals
};

template <GridDirection dir>
constexpr auto numberOfDirs = 0;

template <>
constexpr auto numberOfDirs<GridDirection::WithoutDiagonals> = 4;

template <>
constexpr auto numberOfDirs<GridDirection::WithDiagonals> = 8;

template <GridDirection Diagonals = GridDirection::WithoutDiagonals>
constexpr auto GetAllCardinalDirs() {
  std::array<std::array<int, 2>, numberOfDirs<Diagonals>> ddirs;
  static constexpr std::array<int, 3> ns = {-1, 0, 1};
  int index                              = 0;
  for (const auto& d : ns) {
    for (const auto& dd : ns) {
      if constexpr (Diagonals == GridDirection::WithoutDiagonals) {
        if ((std::abs(d) + std::abs(dd)) != 1) {
          continue;
        }
      }
      if (d == 0 && dd == 0) {
        continue;
      }
      ddirs[index++] = {d, dd};
    }
  }
  return ddirs;
}

enum class HexGridDirections {
  N,
  S,
  NW,
  NE,
  SW,
  SE
};

static const std::unordered_map<HexGridDirections, std::array<int, 3>> HexDirectionMap = {
    {HexGridDirections::N, {-1, 0, 1}},  {HexGridDirections::S, {1, 0, -1}},
    {HexGridDirections::NW, {0, -1, 1}}, {HexGridDirections::NE, {-1, 1, 0}},
    {HexGridDirections::SW, {1, -1, 0}}, {HexGridDirections::SE, {0, 1, -1}}};

struct HexGridStats {
  int r           = 0;
  int q           = 0;
  int s           = 0;
  int distance    = 0;
  int maxDistance = std::numeric_limits<int>::min();
};

template <template <typename> typename Container_T>
inline HexGridStats ParseHexGrid(const Container_T<HexGridDirections>& directions) {
  HexGridStats hexGridStats;

  const auto distanceFunc = [](const auto& r, const auto& q, const auto& s) {
    return (std::abs(r) + std::abs(q) + std::abs(s)) / 2;
  };

  std::for_each(directions.begin(), directions.end(), [&](const auto& hexDirection) {
    const auto& ddir = HexDirectionMap.at(hexDirection);
    hexGridStats.r += ddir[0];
    hexGridStats.q += ddir[1];
    hexGridStats.s += ddir[2];
    hexGridStats.maxDistance = std::max(
        hexGridStats.maxDistance, distanceFunc(hexGridStats.r, hexGridStats.q, hexGridStats.s));
  });
  hexGridStats.distance = distanceFunc(hexGridStats.r, hexGridStats.q, hexGridStats.s);
  return hexGridStats;
}

}  // namespace Grid

namespace Hash {

template <typename T>
inline std::string GetMD5Hash(const T& toHash) {
  unsigned char hashBuf[MD5_DIGEST_LENGTH];
  IGNORE_DEPRECATED_START
  MD5(reinterpret_cast<const unsigned char*>(toHash.c_str()), toHash.size(), hashBuf);
  IGNORE_DEPRECATED_END
  std::ostringstream hashStream;
  hashStream << std::hex << std::setfill('0');
  for (const auto& c : hashBuf) {
    hashStream << std::setw(2) << static_cast<long long>(c);
  }
  return hashStream.str();
}

}  // namespace Hash

namespace Algorithm {

template <typename T>
inline void PrintDeque(const std::deque<T>& dq) {
  auto dqCopy = dq;
  while (!dqCopy.empty()) {
    std::cout << dqCopy.front() << " ";
    dqCopy.pop_front();
  }
  std::cout << '\n';
}

// std::rotate is obnoxiously slow [O(n)] and not specialized for std::deque, quite silly.
template <typename T, typename Int_T>
inline void RotateDeque(std::deque<T>& dq, Int_T numRots) {
  const auto rotLeft = (numRots < 0);
  numRots            = std::abs(numRots);
  if (rotLeft) {
    for (Int_T i = 0; i < numRots; ++i) {
      dq.push_back(std::move(dq.front()));
      dq.pop_front();
    }
  } else {
    for (Int_T i = 0; i < numRots; ++i) {
      dq.push_front(std::move(dq.back()));
      dq.pop_back();
    }
  }
};

// Generate all permutation of a container.
template <typename T>
inline std::vector<T> GetPermutations(const T& inputContainer) {
  auto inputContainerCopy = inputContainer;
  std::sort(inputContainerCopy.begin(), inputContainerCopy.end());
  std::vector<T> perms = {inputContainerCopy};
  while (std::next_permutation(inputContainerCopy.begin(), inputContainerCopy.end())) {
    perms.push_back(inputContainerCopy);
  }
  return perms;
}

// Handles negative mod.
template <typename T>
inline T Modulo(T a, T b) {
  return (b + (a % b)) % b;
}

template <typename T>
inline T Sign(T a) {
  return (a < T{0}) ? T{-1} : T{1};
}

template <typename T, typename Int_T>
inline void CyclicReverse(T& container, Int_T a, Int_T b) {
  const Int_T n = container.size();
  for (; a < b; ++a, --b) {
    std::swap(container[Algorithm::Modulo(a, n)], container[Algorithm::Modulo(b, n)]);
  }
}

}  // namespace Algorithm

namespace DataStructures {

class DSU {
public:
  DSU(int n) : roots(n), sizes(n, 1), numSets(n) { std::iota(roots.begin(), roots.end(), 0); }

  int GetRoot(int n) {
    if (roots[n] == n) {
      return n;
    }
    return roots[n] = GetRoot(roots[n]);
  }

  int GetSize(int n) { return sizes[GetRoot(n)]; }

  int GetNumSets() const { return numSets; }

  void JoinRoots(int a, int b) {
    auto rootA = GetRoot(a);
    auto rootB = GetRoot(b);

    if (rootA == rootB) {
      return;
    }

    if (sizes[rootA] < sizes[rootB]) {
      std::swap(rootA, rootB);
    }

    roots[rootB] = rootA;
    sizes[rootA] += sizes[rootB];
    --numSets;
  }

private:
  std::vector<int> roots;
  std::vector<int> sizes;
  int numSets;
};

}  // namespace DataStructures

#endif  // #ifndef COMMON_H
