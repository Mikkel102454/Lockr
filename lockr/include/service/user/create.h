#ifndef LOCKR_SERVICE_USER_CREATE
#define LOCKR_SERVICE_USER_CREATE 1

#include <string>
#include "nlohmann/json.hpp"

namespace lockr {
    int CreateUser(const std::string &username, const std::string &email,
                   const std::string &password, nlohmann::json &response);
}

#endif