#ifndef STRINGS_H
#define STRINGS_H

/////////////////
//// std
/////////////////
#include <string>

namespace Strings {

inline std::vector<std::string> Split(const std::string& toSplit, const std::string& delim) {
  std::vector<std::string> splitString;
  int p = 0;
  for (auto splitP = toSplit.find(delim, p); splitP != std::string::npos;
       splitP      = toSplit.find(delim, p)) {
    splitString.push_back(toSplit.substr(p, splitP - p));
    p = splitP + 1;
  }
  splitString.push_back(toSplit.substr(p));
  return splitString;
}

};  // namespace Strings

#endif  // #ifndef STRINGS_H
