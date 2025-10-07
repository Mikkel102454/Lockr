#include "service/auth/access.h"

#include "utils/env.h"

#include "jwt-cpp/jwt.h"

using namespace std::chrono;

bool ValidateAccessToken(const string &token) {

}

bool CreateNewAccessToken(const string &refreshToken, string &outToken) {
    outToken = jwt::create()
                .set_issuer("Lockr_Auth")
                .set_type("JWT")
                .set_subject("Userid")
                .set_issued_now()
                .set_expires_at(jwt::date(seconds(600)))
                .sign(jwt::algorithm::hs256(GetEnv("ACCESS_TOKEN_KEY")));
    return true;
}