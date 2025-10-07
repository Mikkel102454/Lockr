#include "service/auth/refresh.h"

#include "db.h"
#include "utils/env.h"

#include <iostream>
#include <bsoncxx/builder/basic/document.hpp>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <sstream>
#include <iomanip>

namespace lockr {
    std::string hashToken(const std::string &token);
    std::string base64url_encode(const unsigned char* data, size_t len);

    bool GenerateRefreshToken(std::string &outToken, const std::string &userId) {
        unsigned char token[32];
        RAND_bytes(token, 32);

        std::string refreshToken = base64url_encode(token, 32);
        outToken = refreshToken;
        const std::string rtHash = hashToken(refreshToken);
        return DB::Insert("token", bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("userId", userId),
                bsoncxx::builder::basic::kvp("refreshToken", rtHash)
        ), nullptr);
    }
    bool InvalidateRefreshToken(const std::string &token) {
        const std::string rtHash = hashToken(token);
        return DB::DeleteOne("token", bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("refreshToken", rtHash)));
    }

    bool InvalidateUserRefreshToken(const std::string &userId) {
        return DB::DeleteAll("token", bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("userId", userId)));
    }

    bool ValidateRefreshToken(const std::string &token){
        const std::string rtHash = hashToken(token);

        if(DB::Exists("token", bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("refreshToken", rtHash)))){
            return true;
        }
        return false;
    }

    std::string GetIdFromRefreshToken(const std::string &token){
        const std::string rtHash = hashToken(token);
        auto doc = DB::getOne("token", bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("refreshToken", rtHash)
        ));
        if(!doc) return "";
        auto v = doc->view();
        auto e = v["userId"];
        if (e && e.type() == bsoncxx::type::k_string) {
            return std::string{ e.get_string().value };
        }
        return "";
    }

// ------ HELPERS -------
    std::string hashToken(const std::string &token) {
        auto toHex = [](const unsigned char* data, size_t len) -> std::string {
            std::ostringstream os;
            os << std::hex << std::setfill('0');
            for (size_t i = 0; i < len; ++i) os << std::setw(2) << static_cast<unsigned>(data[i]);
            return os.str();
        };

        const std::string pepper = GetEnv("REFRESH_TOKEN_KEY");

        if (!pepper.empty()) {
            unsigned char mac[EVP_MAX_MD_SIZE];
            unsigned int macLen = 0;
            HMAC(EVP_sha256(),
                 pepper.data(), static_cast<int>(pepper.size()),
                 reinterpret_cast<const unsigned char*>(token.data()), token.size(),
                 mac, &macLen);
            return toHex(mac, macLen);
        } else {
            unsigned char digest[SHA256_DIGEST_LENGTH];
            SHA256(reinterpret_cast<const unsigned char*>(token.data()), token.size(), digest);
            return toHex(digest, SHA256_DIGEST_LENGTH);
        }
    }


    std::string base64url_encode(const unsigned char* data, size_t len) {
        std::string out;
        out.resize(4 * ((len + 2) / 3));
        int n = EVP_EncodeBlock(reinterpret_cast<unsigned char *>(&out[0]),
                                data, static_cast<int>(len));
        out.resize(n);

        // Make URL-safe and strip padding
        for (char &c: out) {
            if (c == '+') c = '-';
            else if (c == '/') c = '_';
        }
        while (!out.empty() && out.back() == '=') out.pop_back();
        return out;
    }
}