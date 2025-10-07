#ifndef LOCKR_SERVICE_USER_EXIST
#define LOCKR_SERVICE_USER_EXIST 1

#include <string>
#include "nlohmann/json.hpp"

namespace lockr {
    int CheckUsername(const std::string &username, nlohmann::json &response);
    int CheckEmail(const std::string &email, nlohmann::json &response);
}

#endif