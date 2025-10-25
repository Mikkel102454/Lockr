#ifndef LOCKR_SERVICE_AUTH_ACCESS
#define LOCKR_SERVICE_AUTH_ACCESS 1

#include <string>

namespace lockr {
    bool ValidateAccessToken(const std::string &token, std::string& companyId);
    bool ValidateAccessToken(const std::string &token, std::string& companyId, std::string& outUserId);

    int CreateNewAccessToken(std::string &outToken, std::string& companyId, const std::string &userId);
}

#endif