#ifndef CONFIG_H
#define CONFIG_H

/////////////////
//// std
/////////////////
#include <string>

#define DIR std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/') + 1)

static const auto ROOT_DIR = std::string(DIR) + "../";

#endif  // #ifndef CONFIG_H
