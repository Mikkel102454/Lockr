#ifndef LOCKR_SERVICE_AUTH_REFRESH
#define LOCKR_SERVICE_AUTH_REFRESH 1

#include <string>

namespace lockr {
    bool GenerateRefreshToken(std::string &outToken, const std::string &userId, const std::string &companyId);
    bool InvalidateRefreshToken(const std::string &token, const std::string &companyId);
    bool InvalidateUserRefreshToken(const std::string &userUuid);
    bool ValidateRefreshToken(const std::string &token, const std::string &companyId);
    std::string GetIdFromRefreshToken(const std::string &token, const std::string &companyId);
}

#endif