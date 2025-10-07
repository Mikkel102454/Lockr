#ifndef LOCKR_SERVICE_USER_LOGIN
#define LOCKR_SERVICE_USER_LOGIN 1

#include <string>

namespace lockr {
    bool Login(const std::string &email, const std::string &password,
               std::string &outToken);
}

#endif