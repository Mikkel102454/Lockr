#include "service/user/exist.h"

#include "class/user.h"

#include "httplib.h"
#include <string>

namespace lockr {
    int CheckUsername(const std::string &username, nlohmann::json &response) {
        if (username.empty()) {
            response = {
                    {"success", true},
                    {"message", "Username required."}
            };
            return 400;
        }

        if(User::UsernameExist(username)){
            response = {
                    {"success", true},
                    {"exist", true},
                    {"message", "Username is already in use."}
            };
            return 409;
        }
        response = {
                {"success", true},
                {"exist", false},
                {"message", "Username is valid."}
        };
        return 200;
    }


    int CheckEmail(const std::string &email, nlohmann::json &response) {
        if (email.empty()) {
            response = {
                    {"success", true},
                    {"message", "Email required."}
            };
            return 400;
        }

        if(User::EmailExist(email)){
            response = {
                    {"success", true},
                    {"exist", true},
                    {"message", "Email is already in use."}
            };
            return 409;
        }
        response = {
                {"success", true},
                {"exist", false},
                {"message", "Email is valid."}
        };
        return 200;
    }

}