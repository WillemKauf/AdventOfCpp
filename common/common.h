#ifndef COMMON_H
#define COMMON_H

/////////////////
//// md5 lib
/////////////////
#include <openssl/md5.h>

/////////////////
//// std
/////////////////
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
std::string GetMD5Hash(const T& toHash) {
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

#endif  // #ifndef COMMON_H
