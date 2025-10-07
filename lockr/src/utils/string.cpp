#include "utils/string.h"

namespace lockr {
    std::vector<std::string> Split(const std::string& s, char delimiter) {
        std::vector<std::string> tokens;
        std::stringstream ss(s);
        std::string item;
        while (getline(ss, item, delimiter)) {
            tokens.push_back(item);
        }
        return tokens;
    }
}