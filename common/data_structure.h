#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

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

template <typename Container_T, bool Sort = false>
class Counter {
private:
  using FrequencyInteger_type = int;
  using ContainerValue_type   = Container_T::value_type;
  using FrequencyMap_type =
      std::conditional_t<Sort, std::map<ContainerValue_type, FrequencyInteger_type>,
                         std::unordered_map<ContainerValue_type, FrequencyInteger_type>>;
  using value_type     = FrequencyMap_type::value_type;
  using iterator       = FrequencyMap_type::iterator;
  using const_iterator = FrequencyMap_type::const_iterator;
  FrequencyMap_type freqMap;

public:
  Counter(const Container_T& container) {
    for (const auto& v : container) {
      ++freqMap[v];
    }
  }

  FrequencyInteger_type operator[](const ContainerValue_type& k) const { return freqMap[k]; }

  const_iterator begin() const noexcept { return freqMap.begin(); }
  const_iterator end() const noexcept { return freqMap.end(); }

  const_iterator cbegin() const noexcept { return freqMap.cbegin(); }
  const_iterator cend() const noexcept { return freqMap.cend(); }
};

}  // namespace DataStructure

#endif  // #ifndef DATA_STRUCTURE_H
