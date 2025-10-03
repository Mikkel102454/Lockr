#pragma once

#include <string>
extern "C" {
#include "external/libbcrypt/libbcrypt.h"
}

using namespace std;

class User {
public:
    static int hashPassword(const string& password, string& outHash);

    int save();

    static int usernameExist(string username);
    static int emailExist(string email);

    string getUsername() {
        return u_username;
    }
    void setUsername(string username) {
        u_username = username;
    }

    string getEmail() {
        return u_email;
    }
    void setEmail(string email) {
        u_email = email;
    }

    string getPassword() {
        return u_password;
    }
    void setPassword(string password) {
        u_password = password;
    }
private:
    string u_username;
    string u_email;
    string u_password;
};