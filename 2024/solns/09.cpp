/////////////////
//// std
/////////////////
#include <cmath>
#include <complex>
#include <cstddef>
#include <cstdint>
#include <numeric>
#include <queue>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

/////////////////
//// local
/////////////////
#include "../common/advent_base.h"
#include "../common/common.h"
#include "../common/read_input.h"
#include "data_structure.h"
#include "grid.h"
#include "strings.h"

namespace AOC2024 {

struct day_09 : public Advent_type {
  static constexpr int year = 2024;
  static constexpr int date = 9;

  const std::string input = read_lines<std::string>(year, date)[0];

  struct id_and_size {
    int8_t capacity;
    std::vector<int8_t> size;
    std::vector<uint64_t> id;
  };

  static constexpr uint64_t EMPTY_ID = uint64_t(-1);

  using Vec_type = std::vector<id_and_size>;

  Vec_type ParseInput() const {
    std::vector<id_and_size> vec;
    for (const auto [index, size_char] : std::views::enumerate(input)) {
      const bool is_file = (index % 2 == 0);
      const int8_t size  = static_cast<int8_t>(size_char - '0');
      const uint64_t id  = is_file ? static_cast<uint64_t>(index / 2) : EMPTY_ID;
      const std::vector<int8_t> size_vec =
          is_file ? std::vector<int8_t>{size} : std::vector<int8_t>{};
      const std::vector<uint64_t> id_vec =
          is_file ? std::vector<uint64_t>{id} : std::vector<uint64_t>{};
      const int8_t capacity = is_file ? 0 : size;
      vec.emplace_back(capacity, size_vec, id_vec);
    }
    return vec;
  }

  const Vec_type input_map = ParseInput();

  struct part_1_impl {
    static void do_rearrange(int& p1, int& p2, Vec_type& vec) {
      auto& id_and_size_p1 = vec[p1];
      while (p2 > p1 && id_and_size_p1.capacity > 0) {
        auto& id_and_size_p2 = vec[p2];
        auto& capacity_p1    = id_and_size_p1.capacity;
        auto& size_p2        = id_and_size_p2.size.back();
        auto& capacity_p2    = id_and_size_p2.capacity;
        auto id_p2           = id_and_size_p2.id.back();
        auto num_to_move     = std::min(capacity_p1, size_p2);
        capacity_p1 -= num_to_move;
        size_p2 -= num_to_move;
        capacity_p2 += num_to_move;
        id_and_size_p1.id.push_back(id_p2);
        id_and_size_p1.size.push_back(num_to_move);
        if (size_p2 == 0) {
          p2 -= 2;
        }
      }
    }
  };

  struct part_2_impl {
    static void do_rearrange(int& p1, int& p2, Vec_type& vec) {
      auto& id_and_size_p1 = vec[p1];
      p2                   = vec.size() - 1;
      while (p2 > p1 && id_and_size_p1.capacity > 0) {
        auto& id_and_size_p2   = vec[p2];
        auto& capacity_p1      = id_and_size_p1.capacity;
        auto& size_p2          = id_and_size_p2.size.back();
        auto& capacity_p2      = id_and_size_p2.capacity;
        auto id_p2             = id_and_size_p2.id.back();
        const auto num_to_move = size_p2;
        if (num_to_move > 0 && capacity_p1 >= num_to_move) {
          capacity_p1 -= num_to_move;
          size_p2 -= num_to_move;
          capacity_p2 += num_to_move;
          id_and_size_p1.id.push_back(id_p2);
          id_and_size_p1.size.push_back(num_to_move);
        }
        p2 -= 2;
      }
    }
  };

  template <typename impl>
  uint64_t rearrange_impl() const {
    uint64_t res   = 0;
    Vec_type vec   = input_map;
    int p2         = vec.size() - 1;
    int true_index = 0;
    for (int p1 = 0; p1 < vec.size(); ++p1) {
      impl::do_rearrange(p1, p2, vec);

      auto& id_and_size_p1 = vec[p1];
      for (const auto& [id, size] : std::views::zip(id_and_size_p1.id, id_and_size_p1.size)) {
        for (int i = 0; i < size; ++i, ++true_index) {
          res += id * true_index;
        }
      }
      true_index += id_and_size_p1.capacity;
    }
    return res;
  }

  std::string part_1() override { return std::to_string(rearrange_impl<part_1_impl>()); }

  std::string part_2() override { return std::to_string(rearrange_impl<part_2_impl>()); }
};

}  // namespace AOC2024
