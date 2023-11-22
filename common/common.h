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

}  // namespace Algorithm

#endif  // #ifndef COMMON_H
