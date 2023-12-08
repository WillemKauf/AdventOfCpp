#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

/////////////////
//// std
/////////////////
#include <ranges>

namespace DataStructure {

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

template <typename Value_T, typename Int_T, bool Sort>
using OrderingMap_t =
    std::conditional_t<Sort, std::map<Value_T, Int_T>, std::unordered_map<Value_T, Int_T>>;

template <typename Container_T, typename Int_T=int, bool Sort = false>
class Counter {
private:
  using FrequencyInteger_type = Int_T;
  using ContainerValue_type   = Container_T::value_type;
  using FrequencyMap_type     = OrderingMap_t<ContainerValue_type, FrequencyInteger_type, Sort>;
  using value_type            = FrequencyMap_type::value_type;
  using iterator              = FrequencyMap_type::iterator;
  using const_iterator        = FrequencyMap_type::const_iterator;
  using size_type             = FrequencyMap_type::size_type;
  using sorted_by_freq        = std::vector<std::pair<ContainerValue_type, FrequencyInteger_type>>;
  FrequencyMap_type freqMap;

public:
  Counter(const Container_T& container) {
    for (const auto& v : container) {
      ++freqMap[v];
    }
  }

  template<bool Descending=false>
  sorted_by_freq get_sorted_by_freq() const {
    sorted_by_freq freqVals(freqMap.begin(), freqMap.end());
    if constexpr(Descending){
      std::sort(freqVals.begin(), freqVals.end(),
                [](const auto& a, const auto& b) { return a.second > b.second; });
    }else{
      std::sort(freqVals.begin(), freqVals.end(),
                [](const auto& a, const auto& b) { return a.second < b.second; });
    }
    return freqVals;
  }

  FrequencyInteger_type operator[](const ContainerValue_type& k) const { return freqMap[k]; }

  size_type size() const noexcept { return freqMap.size(); }

  const_iterator begin() const noexcept { return freqMap.begin(); }
  const_iterator end() const noexcept { return freqMap.end(); }

  const_iterator cbegin() const noexcept { return freqMap.cbegin(); }
  const_iterator cend() const noexcept { return freqMap.cend(); }
};

// A simple constructor for a container (vector/array) to mapping against monotonic values.
// Is this thing useful for many cases? Not sure yet. Very bare bones as it is right now.
template <typename Container_T, typename Int_T = int, bool Sort = false>
constexpr inline auto MakeOrdering(const Container_T& input) {
  using ContainerValue_type    = Container_T::value_type;
  using OrderingMap_type       = OrderingMap_t<ContainerValue_type, Int_T, Sort>;
  OrderingMap_type orderingMap = {};
  for (const auto& [index, v] : std::views::enumerate(input)) {
    orderingMap[v] = index;
  }
  return orderingMap;
}

}  // namespace DataStructure

#endif  // #ifndef DATA_STRUCTURE_H
