#include "utils/string.h"

#include <bsoncxx/builder/basic/document.hpp>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <sstream>

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

    std::string Base64urlEncode(const unsigned char* data, size_t len) {
        std::string out;
        out.resize(4 * ((len + 2) / 3));
        int n = EVP_EncodeBlock(reinterpret_cast<unsigned char *>(&out[0]),
                                data, static_cast<int>(len));
        out.resize(n);

        for (char &c: out) {
            if (c == '+') c = '-';
            else if (c == '/') c = '_';
        }
        while (!out.empty() && out.back() == '=') out.pop_back();
        return out;
    }
}