#ifndef COMMON_H
#define COMMON_H

enum class GridDirection {
  WithDiagonals,
  WithoutDiagonals
};

template <GridDirection dir>
constexpr auto numberOfDirs() {
  if constexpr (dir == GridDirection::WithDiagonals) {
    return 8;
  } else {
    return 4;
  }
}

template <GridDirection Diagonals = GridDirection::WithoutDiagonals>
constexpr auto getAllCardinalDirs() {
  std::array<std::array<int, 2>, numberOfDirs<Diagonals>()> ddirs;
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

#endif  // #ifndef COMMON_H
