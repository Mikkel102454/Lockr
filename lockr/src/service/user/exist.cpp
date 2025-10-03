#include "service/user/exist.h"

#include "class/user.h"

#include "httplib.h"
#include <string>


int CheckUsername(const string &username, string &response) {
    if (username.empty()) {
        response = R"({"success": false, "message": "Username required"})";
        return 400;
    }

    if(User::usernameExist(username) != 0){
        response = R"({"success": true, "exist": true, "message": "Username is already in use"})";
        return 409;
    }
    response = R"({"success": true, "exist": false, "message": "Username is valid"})";
    return 200;
}


int CheckEmail(const string &email, string &response) {
    if (email.empty()) {
        response = R"({"success": false, "message": "Email required"})";
        return 400;
    }

    if(User::emailExist(email) != 0){
        response = R"({"success": true, "exist": true, "message": "Email is already in use"})";
        return 409;
    }
    response = R"({"success": true, "exist": false, "message": "Email is valid"})";
    return 200;
}
