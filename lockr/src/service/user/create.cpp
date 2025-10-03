#include "service/user/create.h"

#include "httplib.h"
#include "class/user.h"
#include "nlohmann/json.hpp"

using nlohmann::json;
using namespace httplib;

int CreateUser(const std::string &username, const std::string &email, const std::string &password, std::string &response) {
    if (username.empty() || email.empty() || password.empty()) {
        response = "{'success':'false', 'message':'Username, email, password required'}";
        return 400;
    }

    if(User::usernameExist(username) != 0){
        response = "{'success':'false', 'message':'Username already exists'}";
        return 409;
    }

    if(User::emailExist(email) != 0){
        response = "{'success':'false', 'message':'Email already exists'}";
        return 409;
    }

    User user;
    user.setUsername(username);
    user.setEmail(email);

    std::string passwordHash;
    const int rc = User::hashPassword(password, passwordHash);
    if (rc != 0) {
        response = "{'success':'false', 'message':'User could not be created'}";
        return 500;
    }

    user.setPassword(passwordHash);

    if(user.save() == 3) {
        response = "{'success':'false', 'message':'User could not be created'}";
        return 500;
    }

    response = "{'success':'false', 'message':'User has been created'}";
    return 500;
}
