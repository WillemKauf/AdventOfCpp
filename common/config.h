#ifndef CONFIG_H
#define CONFIG_H

/////////////////
//// std
/////////////////
#include <string>

#define DIR std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/') + 1)

static const auto ROOT_DIR = std::string(DIR) + "../";

#if defined(__clang__)
#define COMPILER clang
#elif defined(__GNUC__)
#define COMPILER GCC
#endif

#define STRING2(s) #s
#define STRING(s) STRING2(s)

#define IGNORE_DEPRECATED_START             \
  _Pragma(STRING(COMPILER diagnostic push)) \
      _Pragma(STRING(COMPILER diagnostic ignored "-Wdeprecated-declarations"))

#define IGNORE_DEPRECATED_END _Pragma(STRING(COMPILER diagnostic pop))

#endif  // #ifndef CONFIG_H
