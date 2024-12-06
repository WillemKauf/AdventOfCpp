#ifndef GRID_H
#define GRID_H

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

static constexpr auto OrthogonalDirections = GetAllCardinalDirs();
static constexpr auto DiagonalDirections = GetAllCardinalDirs<Grid::GridDirection::WithDiagonals>();

inline bool InBounds(auto i, auto j, auto n, auto m) {
  return (0 <= i) && (i < n) && (0 <= j) && (j < m);
}

template <typename T>
inline T HashIndex(T i, T j, T n) {
  return T{i + j * n};
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

#endif  // #ifndef GRID_H
