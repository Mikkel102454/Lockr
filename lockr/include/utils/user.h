#pragma once

#include <string>
extern "C" {
#include "external/libbcrypt/libbcrypt.h"
}

class User {
public:
    bool createUser(const std::string& username, const std::string& email, const std::string& password);
private:
    int hashPassword(const std::string& password, std::string& outHash);
    bool save();

    std::string u_username;
    std::string u_email;
    std::string u_password;
};