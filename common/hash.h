#ifndef HASH_H
#define HASH_H

/////////////////
//// std
/////////////////
#include <iomanip>
#include <sstream>

/////////////////
//// md5 lib
/////////////////
#include <openssl/md5.h>

/////////////////
//// local
/////////////////
#include "config.h"

namespace Hash {

template <typename T>
inline std::string GetMD5Hash(const T& toHash) {
  unsigned char hashBuf[MD5_DIGEST_LENGTH];
  IGNORE_DEPRECATED_START
  MD5(reinterpret_cast<const unsigned char*>(toHash.c_str()), toHash.size(), hashBuf);
  IGNORE_DEPRECATED_END
  std::ostringstream hashStream;
  hashStream << std::hex << std::setfill('0');
  for (const auto& c : hashBuf) {
    hashStream << std::setw(2) << static_cast<long long>(c);
  }
  return hashStream.str();
}

}  // namespace Hash


#endif //#ifndef HASH_H
