/////////////////
//// std
/////////////////
#include <limits>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/read_input.h"

namespace AOC2015 {

struct day_15 : public Advent_type {
  static constexpr int year = 2015;
  static constexpr int date = 15;

  struct Cookie {
    Cookie() = default;
    Cookie(const std::string& name, int cap, int dur, int fla, int tex, int cal)
        : name(name), cap(cap), dur(dur), fla(fla), tex(tex), cal(cal) {}
    std::string name;
    int cap = {};
    int dur = {};
    int fla = {};
    int tex = {};
    int cal = {};

    int getScore() const {
      return std::max(0, cap) * std::max(0, dur) * std::max(0, fla) * std::max(0, tex);
    }
  };

  std::vector<Cookie> parseInput() const {
    const std::vector<std::vector<std::string>> rawInput = read_lines_vector_regex<std::string>(
        year, date,
        "([A-Za-z]+): capacity (-?\\d+), durability (-?\\d+), flavor (-?\\d+), texture (-?\\d+), "
        "calories (-?\\d+)");
    std::vector<Cookie> vec;
    vec.reserve(rawInput.size());
    for (const auto& v : rawInput) {
      vec.emplace_back(v[0], std::stoi(v[1]), std::stoi(v[2]), std::stoi(v[3]), std::stoi(v[4]),
                       std::stoi(v[5]));
    }
    return vec;
  }

  const std::vector<Cookie> input = parseInput();
  static constexpr auto maxTsp    = 100;
  const int numCookies            = input.size();

  template <bool calorieCheck = false>
  int recursiveSolve(int cookieIndex, int remainingTsp, const Cookie& cookie) {
    if (remainingTsp == 0 || cookieIndex == numCookies) {
      if constexpr (calorieCheck) {
        if (cookie.cal != 500) {
          return 0;
        }
      }
      return cookie.getScore();
    }
    int mx                 = std::numeric_limits<int>::min();
    const auto& usedCookie = input[cookieIndex];
    for (int i = 0; i <= remainingTsp; ++i) {
      Cookie newCookie = cookie;
      newCookie.cap += i * usedCookie.cap;
      newCookie.dur += i * usedCookie.dur;
      newCookie.fla += i * usedCookie.fla;
      newCookie.tex += i * usedCookie.tex;
      newCookie.cal += i * usedCookie.cal;
      if constexpr (calorieCheck) {
        if (newCookie.cal > 500) {
          continue;
        }
      }
      mx = std::max(mx, recursiveSolve<calorieCheck>(cookieIndex + 1, remainingTsp - i, newCookie));
    }

    return mx;
  }

  std::string part_1() override { return std::to_string(recursiveSolve(0, maxTsp, {})); }

  std::string part_2() override { return std::to_string(recursiveSolve<true>(0, maxTsp, {})); }
};

};  // namespace AOC2015
