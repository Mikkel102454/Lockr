#include "service/auth/refresh.h"

#include "db.h"
#include "utils/env.h"
#include "utils/hash.h"
#include "utils/string.h"

#include <bsoncxx/builder/basic/document.hpp>
#include <openssl/rand.h>

namespace lockr {
    bool GenerateRefreshToken(std::string &outToken, const std::string &userId) {
        unsigned char token[32];
        RAND_bytes(token, 32);

        std::string refreshToken = Base64urlEncode(token, 32);
        outToken = refreshToken;
        const std::string rtHash = Hash(refreshToken, "REFRESH_TOKEN_KEY");
        return DB::Insert("token", bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("user_id", userId),
                bsoncxx::builder::basic::kvp("refresh_token", rtHash)
        ), nullptr);
    }
    bool InvalidateRefreshToken(const std::string &token) {
        const std::string rtHash = Hash(token, "REFRESH_TOKEN_KEY");
        return DB::DeleteOne("token", bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("refresh_token", rtHash)));
    }

    bool InvalidateUserRefreshToken(const std::string &userId) {
        return DB::DeleteAll("token", bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("user_id", userId)));
    }

    bool ValidateRefreshToken(const std::string &token){
        const std::string rtHash = Hash(token, "REFRESH_TOKEN_KEY");

        if(DB::Exists("token", bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("refresh_token", rtHash)))){
            return true;
        }
        return false;
    }

    std::string GetIdFromRefreshToken(const std::string &token){
        const std::string rtHash = Hash(token, "REFRESH_TOKEN_KEY");
        auto doc = DB::getOne("token", bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("refresh_token", rtHash)
        ));
        if(!doc) return "";
        auto v = doc->view();
        auto e = v["user_id"];
        if (e && e.type() == bsoncxx::type::k_string) {
            return std::string{ e.get_string().value };
        }
        return "";
    }
}