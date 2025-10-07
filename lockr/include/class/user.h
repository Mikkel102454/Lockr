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

        std::string_view getUsername() { return m_username; }
        void setUsername(const std::string& username) { m_username = username; }

        std::string_view getEmail() { return m_email; }
        void setEmail(const std::string& email) { m_email = email; }

        std::string_view getPassword() { return m_password; }
        void setPassword(const std::string& password) { m_password = password; }
    private:
        std::string m_username;
        std::string m_email;
        std::string m_password;
    };
}

#endif