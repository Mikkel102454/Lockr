#ifndef LOCKR_SERVICE_AUTH_REFRESH
#define LOCKR_SERVICE_AUTH_REFRESH 1

#include <string>

namespace lockr {
    bool GenerateRefreshToken(std::string &outToken, const std::string &userId);
    bool InvalidateRefreshToken(const std::string &token);
    bool InvalidateUserRefreshToken(const std::string &userUuid);
    bool ValidateRefreshToken(const std::string &token);
    std::string GetIdFromRefreshToken(const std::string &token);
}

#endif