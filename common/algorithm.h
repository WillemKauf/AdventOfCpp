#ifndef ALGORITHM_H
#define ALGORITHM_H

/////////////////
//// std
/////////////////
#include <algorithm>
#include <bitset>
#include <deque>
#include <limits>

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

}  // namespace Algorithm

#endif  // #ifndef ALGORITHM_H
