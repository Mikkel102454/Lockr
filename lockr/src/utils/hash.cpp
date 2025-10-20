#include "utils/env.h"

#include "string"
#include <bsoncxx/builder/basic/document.hpp>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

namespace lockr {
    std::string Hash(const std::string &value) {
        auto toHex = [](const unsigned char* data, size_t len) -> std::string {
            std::ostringstream os;
            os << std::hex << std::setfill('0');
            for (size_t i = 0; i < len; ++i) os << std::setw(2) << static_cast<unsigned>(data[i]);
            return os.str();
        };

        const std::string pepper = GetEnv("HASH_KEY");

        if (!pepper.empty()) {
            unsigned char mac[EVP_MAX_MD_SIZE];
            unsigned int macLen = 0;
            HMAC(EVP_sha256(),
                 pepper.data(), static_cast<int>(pepper.size()),
                 reinterpret_cast<const unsigned char*>(value.data()), value.size(),
                 mac, &macLen);
            return toHex(mac, macLen);
        } else {
            unsigned char digest[SHA256_DIGEST_LENGTH];
            SHA256(reinterpret_cast<const unsigned char*>(value.data()), value.size(), digest);
            return toHex(digest, SHA256_DIGEST_LENGTH);
        }
    }

    std::string Hash(const std::string &value, const std::string &customKey) {
        auto toHex = [](const unsigned char* data, size_t len) -> std::string {
            std::ostringstream os;
            os << std::hex << std::setfill('0');
            for (size_t i = 0; i < len; ++i) os << std::setw(2) << static_cast<unsigned>(data[i]);
            return os.str();
        };

        const std::string pepper = GetEnv(customKey);

        if (!pepper.empty()) {
            unsigned char mac[EVP_MAX_MD_SIZE];
            unsigned int macLen = 0;
            HMAC(EVP_sha256(),
                 pepper.data(), static_cast<int>(pepper.size()),
                 reinterpret_cast<const unsigned char*>(value.data()), value.size(),
                 mac, &macLen);
            return toHex(mac, macLen);
        } else {
            unsigned char digest[SHA256_DIGEST_LENGTH];
            SHA256(reinterpret_cast<const unsigned char*>(value.data()), value.size(), digest);
            return toHex(digest, SHA256_DIGEST_LENGTH);
        }
    }
}