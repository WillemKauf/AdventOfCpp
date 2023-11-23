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

}  // namespace DataStructure
