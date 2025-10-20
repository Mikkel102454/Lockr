#ifndef LOCKR_UTILS_HASH
#define LOCKR_UTILS_HASH 1

#include <string>

namespace lockr {
    std::string Hash(const std::string &value);
    std::string Hash(const std::string &value, const std::string &customKey);
}

#endif