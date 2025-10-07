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

    static string GetUserId(const string &email);
    static string
    GetIdFromRefreshToken(const string &token);

    static bool ValidateRefreshToken(const string &token);
    static bool DeleteRefreshToken(const string &token);
    static bool DeleteAllRefreshToken(const string &userId);
    static bool InsertRefreshToken(string &token, const string &userId);

    static bool Authenticate(const string &email, const string &password);

    string getUsername() {
        return u_username;
    }
    void setUsername(const string &username) {
        u_username = username;
    }

    string getEmail() {
        return u_email;
    }
    void setEmail(const string &email) {
        u_email = email;
    }

    string getPassword() {
        return u_password;
    }
    void setPassword(const string &password) {
        u_password = password;
    }
private:
    static string hashToken(const string &token);

    string u_username;
    string u_email;
    string u_password;
};