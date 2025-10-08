#ifndef LOCKR_CLASS_USER
#define LOCKR_CLASS_USER 1

#include <string>
extern "C" {
#include "external/libbcrypt/libbcrypt.h"
}

namespace lockr {
    class User {
    public:
        static int HashPassword(const std::string& password, std::string& outHash);

        static bool UsernameExist(const std::string& username);
        static bool EmailExist(const std::string& email);

        static std::string GetUserId(const std::string& email);

        static bool Authenticate(const std::string& email, const std::string& password);

        int save();

        std::string_view getUsername() { return mUsername; }
        void setUsername(const std::string& username) { mUsername = username; }

        std::string_view getEmail() { return mEmail; }
        void setEmail(const std::string& email) { mEmail = email; }

        std::string_view getPassword() { return mPassword; }
        void setPassword(const std::string& password) { mPassword = password; }
    private:
        std::string mUsername;
        std::string mEmail;
        std::string mPassword;
    };
}

#endif