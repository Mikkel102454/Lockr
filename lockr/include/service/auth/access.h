#ifndef LOCKR_SERVICE_AUTH_ACCESS
#define LOCKR_SERVICE_AUTH_ACCESS 1

#include <string>

namespace lockr {
    bool ValidateAccessToken(const std::string &token);
    bool ValidateAccessToken(const std::string &token, std::string& outUserId);

    int CreateNewAccessToken(std::string &outToken, const std::string &userId);
}

#endif