#include "service/auth/access.h"

#include "utils/env.h"
#include "class/user.h"

#include "jwt-cpp/jwt.h"
#include "httplib.h"

using namespace std::chrono;
using namespace httplib;

bool ValidateAccessToken(const string &token) {
    return true;
}

int CreateNewAccessToken(const string &refreshToken, string &outToken) {

    if(!User::ValidateRefreshToken(refreshToken)) return 409;

    outToken = jwt::create()
                .set_issuer("Lockr_Auth")
                .set_type("JWT")
                .set_subject("Userid")
                .set_issued_now()
                .set_expires_at(jwt::date(seconds(600)))
                .sign(jwt::algorithm::hs256(GetEnv("ACCESS_TOKEN_KEY")));
    return OK_200;
}