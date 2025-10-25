#include "service/user/login.h"

#include "class/user.h"
#include "service/auth/refresh.h"

namespace lockr {
    bool Login(const std::string& email, const std::string& password, const std::string& companyId, std::string& outToken) {
        if (!User::Authenticate(email, password)) {
            return false;
        }

        GenerateRefreshToken(outToken, User::GetUserId(email), companyId);
        return true;
    }

}