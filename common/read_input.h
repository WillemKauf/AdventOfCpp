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
#include <vector>

/////////////////
//// local
/////////////////
#include "config.h"

inline auto get_input_file(int year, int date) {
  return ROOT_DIR + std::to_string(year) + "/input/input" + std::to_string(date) + ".txt";
}

inline auto read_lines_string(int year, int date) {
  const std::string input_file = get_input_file(year, date);
  std::vector<std::string> input;
  {
    std::ifstream in_file(input_file);
    if (!in_file.is_open()) {
      throw std::runtime_error("Couldn't open file" + input_file);
    }
    for (std::string line; std::getline(in_file, line);) {
      std::istringstream iss(line);
      input.push_back(line);
    }
  }
  return input;
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

template <typename T>
inline auto read_lines_vector_regex(int year, int date, const std::string& regex_str) {
  const std::string input_file = get_input_file(year, date);
  boost::regex regex(regex_str);
  std::vector<std::vector<T>> input;
  {
    std::ifstream in_file(input_file);
    if (!in_file.is_open()) {
      throw std::runtime_error("Couldn't open file" + input_file);
    }
    for (std::string line; std::getline(in_file, line);) {
      std::istringstream iss(line);
      std::vector<T> line_vec;
      std::string string = iss.str();
      boost::sregex_iterator iterator(string.begin(), string.end(), regex);
      while (iterator != boost::sregex_iterator()) {
        boost::smatch match = *iterator;
        if (match.size() == 1) {
          if constexpr (std::is_same_v<T, int>) {
            line_vec.push_back(std::stoi(match.str()));
          } else {
            line_vec.push_back(match.str());
          }
        } else {
          for (int i = 1; i < match.size(); ++i) {
            const auto& group = match[i];
            if (group.length() == 0) continue;
            if constexpr (std::is_same_v<T, int>) {
              line_vec.push_back(std::stoi(group.str()));
            } else {
              line_vec.push_back(group.str());
            }
          }
        }
        ++iterator;
      }
      input.push_back(line_vec);
    }
  }
  return input;
}

template <typename T>
inline auto read_lines_vector_regex(const std::string& input_file, const std::string& regex_str) {
  boost::regex regex(regex_str);
  std::vector<std::vector<T>> input;
  {
    std::ifstream in_file(input_file);
    if (!in_file.is_open()) {
      throw std::runtime_error("Couldn't open file" + input_file);
    }
    for (std::string line; std::getline(in_file, line);) {
      std::istringstream iss(line);
      std::vector<T> line_vec;
      std::string string = iss.str();
      boost::sregex_iterator iterator(string.begin(), string.end(), regex);
      while (iterator != boost::sregex_iterator()) {
        boost::smatch match = *iterator;
        if (match.size() == 1) {
          if constexpr (std::is_same_v<T, int>) {
            line_vec.push_back(std::stoi(match.str()));
          } else {
            line_vec.push_back(match.str());
          }
        } else {
          for (int i = 1; i < match.size(); ++i) {
            const auto& group = match[i];
            if (group.length() == 0) continue;
            if constexpr (std::is_same_v<T, int>) {
              line_vec.push_back(std::stoi(group.str()));
            } else {
              line_vec.push_back(group.str());
            }
          }
        }
        ++iterator;
      }
      input.push_back(line_vec);
    }
  }
  return input;
}

template <typename T, typename... Ts>
inline auto read_lines_vector_regex(int year, int date, const Ts&... regex_strs) {
  const std::string input_file = get_input_file(year, date);
  std::vector<boost::regex> regexVec{boost::regex(regex_strs)...};
  std::vector<std::vector<T>> input;
  {
    std::ifstream in_file(input_file);
    if (!in_file.is_open()) {
      throw std::runtime_error("Couldn't open file" + input_file);
    }
    for (std::string line; std::getline(in_file, line);) {
      std::istringstream iss(line);
      std::vector<T> line_vec;
      std::string string = iss.str();
      for (int i = 0; const auto& regex : regexVec) {
        boost::sregex_iterator iterator(string.begin(), string.end(), regex);
        const auto shouldBreak = iterator != boost::sregex_iterator();
        while (iterator != boost::sregex_iterator()) {
          boost::smatch match = *iterator;
          if (match.size() == 1) {
            if constexpr (std::is_same_v<T, int>) {
              line_vec.push_back(std::stoi(match.str()));
            } else {
              line_vec.push_back(match.str());
            }
          } else if (match.size() > 1) {
            for (int i = 1; i < match.size(); ++i) {
              const auto& group = match[i];
              if (group.length() == 0) continue;
              if constexpr (std::is_same_v<T, int>) {
                line_vec.push_back(std::stoi(group.str()));
              } else {
                line_vec.push_back(group.str());
              }
            }
          }
          ++iterator;
        }
        if (shouldBreak) {
          break;
        }
        ++i;
      }
      input.push_back(line_vec);
    }
  }
  return input;
}

template <typename T, typename... Ts>
inline auto read_lines_vector_regex(const std::string& input_file, const Ts&... regex_strs) {
  std::vector<boost::regex> regexVec{boost::regex(regex_strs)...};
  std::vector<std::vector<T>> input;
  {
    std::ifstream in_file(input_file);
    if (!in_file.is_open()) {
      throw std::runtime_error("Couldn't open file" + input_file);
    }
    for (std::string line; std::getline(in_file, line);) {
      std::istringstream iss(line);
      std::vector<T> line_vec;
      std::string string = iss.str();
      for (int i = 0; const auto& regex : regexVec) {
        boost::sregex_iterator iterator(string.begin(), string.end(), regex);
        const auto shouldBreak = iterator != boost::sregex_iterator();
        while (iterator != boost::sregex_iterator()) {
          boost::smatch match = *iterator;
          if (match.size() == 1) {
            if constexpr (std::is_same_v<T, int>) {
              line_vec.push_back(std::stoi(match.str()));
            } else {
              line_vec.push_back(match.str());
            }
          } else if (match.size() > 1) {
            for (int i = 1; i < match.size(); ++i) {
              const auto& group = match[i];
              if (group.length() == 0) continue;
              if constexpr (std::is_same_v<T, int>) {
                line_vec.push_back(std::stoi(group.str()));
              } else {
                line_vec.push_back(group.str());
              }
            }
          }
          ++iterator;
        }
        if (shouldBreak) {
          break;
        }
        ++i;
      }
      input.push_back(line_vec);
    }
  }
  return input;
}

#endif
