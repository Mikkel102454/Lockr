#ifndef LOCKR_UTILS_ENV
#define LOCKR_UTILS_ENV 1

#include <string>

namespace lockr {
    void InitDotEnv();
    std::string GetEnv(const std::string& key);
}

#endif