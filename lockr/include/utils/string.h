#ifndef LOCKR_UTILS_STRING
#define LOCKR_UTILS_STRING 1

#include <string>
#include <vector>
#include <sstream>

namespace lockr {
    std::vector<std::string> Split(const std::string& s, char delimiter);

    std::string Base64urlEncode(const unsigned char* data, size_t len);
}

#endif