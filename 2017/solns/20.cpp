/////////////////
//// std
/////////////////
#include <valarray>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"

namespace AOC2017 {

struct day_20 : public Advent_type {
  static constexpr int year                 = 2017;
  static constexpr int date                 = 20;
  const std::vector<std::vector<int>> input = read_lines_vector_regex<int>(year, date, "(-?\\d+)");

  struct Particle {
    using Vector3d = std::valarray<int>;
    Particle(int id, const Vector3d& p, const Vector3d& v, const Vector3d& a)
        : id(id), p(p), v(v), a(a) {}
    int id;
    Vector3d p;
    Vector3d v;
    Vector3d a;

    auto operator<=>(const Particle& other) const {
      const auto magSum       = [](const auto& vArr) { return std::abs(vArr).sum(); };
      const auto aMagSum      = magSum(a);
      const auto vMagSum      = magSum(v);
      const auto pMagSum      = magSum(p);
      const auto otherAMagSum = magSum(other.a);
      const auto otherVMagSum = magSum(other.v);
      const auto otherPMagSum = magSum(other.p);
      return std::tie(aMagSum, vMagSum, pMagSum) <=>
             std::tie(otherAMagSum, otherVMagSum, otherPMagSum);
    }
  };

  std::vector<Particle> ParseInput() const {
    std::vector<Particle> particles;
    for (auto [id, v] : std::views::enumerate(input)) {
      particles.emplace_back(id, Particle::Vector3d{v[0], v[1], v[2]},
                             Particle::Vector3d{v[3], v[4], v[5]},
                             Particle::Vector3d{v[6], v[7], v[8]});
    }

    return particles;
  }

  const std::vector<Particle> inputParticles = ParseInput();

  std::string part_1() override {
    auto particles = inputParticles;
    std::sort(particles.begin(), particles.end());
    return std::to_string(particles[0].id);
  }

  std::string part_2() override {
    auto particles = inputParticles;

    const auto toString = [](const auto& v) {
      return std::to_string(v[0]) + "|" + std::to_string(v[1]) + "|" + std::to_string(v[2]);
    };

    int lastCollide = {};
    std::unordered_map<std::string, int> seenPos;
    for (int t = 0; true; ++t) {
      if (t - lastCollide > 10) {
        break;
      }
      for (auto& particle : particles) {
        particle.v += particle.a;
        particle.p += particle.v;
        ++seenPos[toString(particle.p)];
      }
      std::vector<Particle> newParticles;
      for (const auto& particle : particles) {
        if (seenPos[toString(particle.p)] == 1) {
          newParticles.push_back(particle);
        }
      }
      lastCollide = (particles.size() != newParticles.size()) ? t : lastCollide;
      particles   = newParticles;
    }
    return std::to_string(particles.size());
  }
};

};  // namespace AOC2017
