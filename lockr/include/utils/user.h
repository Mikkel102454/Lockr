#pragma once

#include <string>
extern "C" {
#include "external/libbcrypt/libbcrypt.h"
}

class User {
public:
    static int hashPassword(const std::string& password, std::string& outHash);

    int save();

    static int usernameExist(std::string username);
    static int emailExist(std::string email);

    std::string getUsername() {
        return u_username;
    }
    void setUsername(std::string username) {
        u_username = username;
    }

    std::string getEmail() {
        return u_email;
    }
    void setEmail(std::string email) {
        u_email = email;
    }

    std::string getPassword() {
        return u_password;
    }
    void setPassword(std::string password) {
        u_password = password;
    }
private:
    std::string u_username;
    std::string u_email;
    std::string u_password;
};