#ifndef READ_INPUT_H
#define READ_INPUT_H

/////////////////
//// boost regex
/////////////////
#include <boost/next_prior.hpp>
#include <boost/regex.hpp>

/////////////////
//// std
/////////////////
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

/////////////////
//// local
/////////////////
#include "advent_base.h"
#include "config.h"

inline auto get_input_file(int year, int date) {
  return ROOT_DIR + std::to_string(year) + "/input/input" + std::to_string(date) + ".txt";
}

template <typename T>
inline auto read_lines(int year, int date) {
  const std::string input_file = get_input_file(year, date);
  std::vector<T> input;
  {
    std::ifstream in_file(input_file);
    if (!in_file.is_open()) {
      throw std::runtime_error("Couldn't open file" + input_file);
    }
    for (std::string line; std::getline(in_file, line);) {
      std::istringstream iss(line);
      if constexpr (std::is_same_v<T, int>) {
        input.push_back(std::stoi(line));
      } else if constexpr (std::is_same_v<T, char>) {
        for (const auto& c : line) {
          input.push_back(c);
        }
      } else {
        input.push_back(line);
      }
    }
  }
  return input;
}

inline auto read_single_line(int year, int date) {
  const std::string input_file = get_input_file(year, date);
  std::vector<char> input;
  {
    std::ifstream in_file(input_file);
    if (!in_file.is_open()) {
      throw std::runtime_error("Couldn't open file" + input_file);
    }
    for (std::string line; std::getline(in_file, line);) {
      std::istringstream string(line);
      std::string s;
      while (string >> s) {
        for (const auto& c : s) {
          input.push_back(c);
        }
      }
    }
  }
  return input;
}

inline auto read_lines_vector(int year, int date) {
  const std::string input_file = get_input_file(year, date);
  std::vector<std::vector<char>> input;
  {
    std::ifstream in_file(input_file);
    if (!in_file.is_open()) {
      throw std::runtime_error("Couldn't open file" + input_file);
    }
    for (std::string line; std::getline(in_file, line);) {
      std::istringstream string(line);
      std::vector<char> line_vec;
      std::string s;
      while (string >> s) {
        for (const auto& c : s) {
          line_vec.push_back(c);
        }
      }
      input.push_back(line_vec);
    }
  }
  return input;
}

// Concept to check if a type has a static bool collect_position
template <typename T>
concept HasCollectPosition = requires {
  { T::collect_position } -> std::convertible_to<const bool>;
};

template <typename T>
T string_to_value_type(const std::string& str) {
  if constexpr (std::is_same_v<T, int>) {
    return std::stoi(str);
  } else if constexpr (std::is_same_v<T, long>) {
    return std::stol(str);
  } else if constexpr (std::is_same_v<T, uint32_t>) {
    return std::stoul(str);
  } else if constexpr (std::is_same_v<T, uint64_t>) {
    return std::stoull(str);
  } else if constexpr (std::is_same_v<T, char>) {
    return str[0];
  } else {
    return str;
  }
}

template <typename T, bool Flatten = false, typename... Ts>
inline auto read_lines_vector_regex(const std::string& input_file, Ts&&... regex_strs) {
  using Return_type = std::conditional_t<Flatten, std::vector<T>, std::vector<std::vector<T>>>;
  Return_type input;
  {
    std::vector<boost::regex> regexVec{boost::regex(std::forward<Ts>(regex_strs))...};
    std::ifstream in_file(input_file);
    if (!in_file.is_open()) {
      throw std::runtime_error("Couldn't open file" + input_file);
    }
    for (std::string line; std::getline(in_file, line);) {
      std::istringstream iss(line);
      std::vector<T> line_vec;
      std::string string = iss.str();
      for (const auto& regex : regexVec) {
        boost::sregex_iterator iterator(string.begin(), string.end(), regex);
        while (iterator != boost::sregex_iterator()) {
          boost::smatch match = *iterator;
          if (match.size() == 1) {
            if constexpr (HasCollectPosition<T>) {
              T res;
              res.v = string_to_value_type<typename T::Value_type>(match.str());
              res.p = match.position();
              line_vec.push_back(std::move(res));
            } else {
              line_vec.push_back(string_to_value_type<T>(match.str()));
            }
          } else if (match.size() > 1) {
            for (int i = 1; i < match.size(); ++i) {
              const auto& group = match[i];
              if (group.length() == 0) continue;
              if constexpr (HasCollectPosition<T>) {
                T res;
                res.v = string_to_value_type<typename T::Value_type>(group.str());
                res.p = match.position();
                line_vec.push_back(std::move(res));
              } else {
                line_vec.push_back(string_to_value_type<T>(group.str()));
              }
            }
          }
          ++iterator;
        }
      }

      if (!line_vec.empty()) {
        if constexpr (Flatten) {
          for (auto& line : line_vec) {
            input.push_back(std::move(line));
          }
        } else {
          input.push_back(std::move(line_vec));
        }
      }
    }
  }
  return input;
}

template <typename T, bool Flatten = false, typename... Ts>
inline auto read_lines_vector_regex(int year, int date, Ts&&... regex_strs) {
  const std::string input_file = get_input_file(year, date);
  return read_lines_vector_regex<T, Flatten>(input_file, std::forward<Ts>(regex_strs)...);
}

#endif
