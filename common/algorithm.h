#ifndef ALGORITHM_H
#define ALGORITHM_H

/////////////////
//// std
/////////////////
#include <algorithm>
#include <bitset>
#include <cmath>
#include <deque>
#include <functional>
#include <limits>
#include <ranges>

namespace Algorithm {

template <typename Container_T, typename Int_T = Container_T::value_type>
inline Int_T LCM(const Container_T& container) {
  return std::accumulate(container.begin(), container.end(), Int_T(1),
                         [](const auto& a, const auto& b) { return std::lcm(a, b); });
}

template <typename Container_T, typename Int_T = int>
inline Int_T Sum(const Container_T& container) {
  return std::accumulate(container.begin(), container.end(), Int_T(0));
}

template <typename Container_T, typename Int_T = int>
inline Int_T Product(const Container_T& container) {
  using value_type = Container_T::value_type;
  return std::accumulate(container.begin(), container.end(), Int_T(1),
                         std::multiplies<value_type>{});
}

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
  const auto dqSize  = dq.size();

  if (dqSize == 0) {
    return;
  }

  numRots = std::abs(numRots) % dqSize;
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

namespace {
template <typename T, typename Int_T>
inline std::vector<T> GetCombinationsImpl(const T& inputContainer, Int_T k, std::string& selected,
                                          T& currComb, std::unordered_set<std::string>& seenCombs) {
  if (k == 0) {
    if (!seenCombs.contains(selected)) {
      seenCombs.insert(selected);
      return {currComb};
    } else {
      return {};
    }
  }
  std::vector<T> combinations;
  for (const auto& [index, v] : std::views::enumerate(inputContainer)) {
    if (selected[index] == 'Y') {
      continue;
    }
    selected[index] = 'Y';
    currComb.push_back(v);
    for (const auto& combination :
         GetCombinationsImpl(inputContainer, k - 1, selected, currComb, seenCombs)) {
      combinations.push_back(combination);
    }
    selected[index] = 'N';
    currComb.pop_back();
  }
  return combinations;
}
}  // namespace

template <typename T, typename Int_T>
inline std::vector<T> GetCombinations(const T& inputContainer, Int_T k) {
  std::unordered_set<std::string> seenCombs;
  std::vector<T> combinations;
  std::string selected(inputContainer.size(), 'N');
  T currComb;
  currComb.reserve(k);
  for (const auto& combination :
       GetCombinationsImpl(inputContainer, k, selected, currComb, seenCombs)) {
    combinations.push_back(combination);
  }
  return combinations;
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

template <int fillWidth = 2, typename T>
inline std::string ToHex(const T& t) {
  std::stringstream ss;
  for (const auto& v : t) {
    ss << std::hex << std::setfill('0') << std::setw(fillWidth) << v;
  }
  return ss.str();
}

template <int n, typename T>
inline std::bitset<n> HexToBin(const T& t) {
  std::stringstream ss;
  ss << std::hex << t;
  unsigned int num;
  ss >> num;
  std::bitset<n> b(num);
  return b;
}

template <typename Int_T = int>
Int_T HexToDec(std::string_view hex) {
  Int_T dec;
  std::stringstream ss;
  ss << std::hex << hex;
  ss >> dec;
  return dec;
}

template <typename Set_type>
inline Set_type SetIntersection(const Set_type& sOne, const Set_type& sTwo) {
  Set_type setIntersection;
  const bool useSOne    = (sOne.size() < sTwo.size());
  const auto& sIterate  = (useSOne) ? sOne : sTwo;
  const auto& sContains = (useSOne) ? sTwo : sOne;
  for (const auto& v : sIterate) {
    if (sContains.contains(v)) {
      setIntersection.insert(v);
    }
  }
  return setIntersection;
}

template <typename Set_type>
inline Set_type SetDifference(const Set_type& sOne, const Set_type& sTwo) {
  Set_type setDifference;
  for (const auto& v : sOne) {
    if (!sTwo.contains(v)) {
      setDifference.insert(v);
    }
  }
  return setDifference;
}

template <typename Set_type>
inline Set_type SetUnion(const Set_type& sOne, const Set_type& sTwo) {
  Set_type setUnion;
  for (const auto& v : sOne) {
    setUnion.insert(v);
  }
  for (const auto& v : sTwo) {
    setUnion.insert(v);
  }
  return setUnion;
}

template <typename Grid_type>
inline Grid_type TransposeGrid(const Grid_type& grid) {
  const auto m = grid.size();
  const auto n = grid[0].size();
  Grid_type cols(n);
  for (auto& col : cols) {
    col.reserve(m);
  }
  for (const auto& row : grid) {
    for (const auto& [i, c] : std::views::enumerate(row)) {
      cols[i].push_back(c);
    }
  }
  return cols;
}

template <typename T>
inline T DigitCount(T n) {
  return n > 0 ? static_cast<int>(std::log10(n)) + 1 : 1;
}

}  // namespace Algorithm

#endif  // #ifndef ALGORITHM_H
