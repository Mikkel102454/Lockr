#include "service/auth/access.h"

#include "utils/env.h"

#include "jwt-cpp/jwt.h"
#include "httplib.h"

namespace lockr {
    bool ValidateAccessToken(const std::string &token) {
        return true;
    }

    int CreateNewAccessToken(std::string &outToken, const std::string &userId) {

        outToken = jwt::create()
                .set_issuer("Lockr_Auth")
                .set_type("JWT")
                .set_subject(userId)
                .set_issued_now()
                .set_expires_at(jwt::date(std::chrono::seconds(600)))
                .sign(jwt::algorithm::hs256(GetEnv("ACCESS_TOKEN_KEY")));
        return httplib::OK_200;
    }
}