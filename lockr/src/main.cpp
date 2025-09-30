#include "httplib.h"
#include "utils/config.h"
#include <string>
#include "lockr/db.h"
#include "utils/user.h"
#include "nlohmann/json.hpp"

using nlohmann::json;
using namespace httplib;

int main() {
    Server svr;

    //init db
    Config::initialize();
    DB::connect();

    svr.Post("/api/users/create", [](const Request& req, Response& res) {
        json body = json::parse(req.body, /*callback*/nullptr, /*allow_exceptions*/false);
        if (body.is_discarded()) {
            res.status = BadRequest_400;
            res.set_content(R"({"success":"false", "message":"Invalid JSON"})", "application/json");
            return;
        }

        std::string username = body.value("username", "");
        std::string email    = body.value("email", "");
        std::string password = body.value("password", "");

        if (username.empty() || email.empty() || password.empty()) {
            res.status = BadRequest_400;
            res.set_content(R"({"success":"false", "message":"Username, email, password required"})",
                            "application/json");
            return;
        }

        if(User::usernameExist(username) != 0){
            res.status = Conflict_409;
            res.set_content(R"({"success":"false", "message":"Username already exists"})",
                            "application/json");
            return;
        }

        if(User::emailExist(email) != 0){
            res.status = Conflict_409;
            res.set_content(R"({"success":"false", "message":"Email already exists"})",
                            "application/json");
            return;
        }

        User user;
        user.setUsername(username);
        user.setEmail(email);

        std::string passwordHash;
        const int rc = User::hashPassword(password, passwordHash);
        if (rc != 0) {
            res.status = InternalServerError_500;
            res.set_content(R"({"success":"false", "message":"User could not be created"})", "application/json");
            return;
        }

        user.setPassword(passwordHash);

        int ec = user.save();
        if (ec == 1) {
            res.status = Conflict_409;
            res.set_content(R"({"success":"false", "message":"Username already exists"})", "application/json");
            return;
        } else if(ec == 2) {
            res.status = Conflict_409;
            res.set_content(R"({"success":"false", "message":"Email already exists"})", "application/json");
            return;
        } else if(ec == 3) {
            res.status = InternalServerError_500;
            res.set_content(R"({"success":"false", "message":"User could not be created"})", "application/json");
            return;
        }

        res.status = OK_200;
        res.set_content(R"({"success":"true", "message":"User has been created"})", "application/json");
    });

    svr.Get("/api/users/username", [](const Request& req, Response& res) {
        if (!req.has_param("username")) {
            res.status = 400;
            res.set_content(R"({"success":false,"message":"Username required"})", "application/json");
            return;
        }

        std::string username = req.get_param_value("username");

        if (username.empty()) {
            res.status = BadRequest_400;
            res.set_content(R"({"success":"false", "message":"Username required"})",
                            "application/json");
            return;
        }

        if(User::usernameExist(username) != 0){
            res.status = Conflict_409;
            res.set_content(R"({"success":"true", "exists":"true", "message":"Username is already taken"})",
                            "application/json");
            return;
        }

        res.status = OK_200;
        res.set_content(R"({"success":"true", "exists":"false", "message":"Username is available"})",
                        "application/json");
    });
    svr.Get("/api/users/email", [](const Request& req, Response& res) {
        if (!req.has_param("email")) {
            res.status = 400;
            res.set_content(R"({"success":false,"message":"Email required"})", "application/json");
            return;
        }

        std::string email = req.get_param_value("email");

        if (email.empty()) {
            res.status = BadRequest_400;
            res.set_content(R"({"success":"false", "message":"Email required"})",
                            "application/json");
            return;
        }

        if(User::emailExist(email) != 0){
            res.status = Conflict_409;
            res.set_content(R"({"success":"true", "exists":"true", "message":"Email has already been registered"})",
                            "application/json");
            return;
        }

        res.status = OK_200;
        res.set_content(R"({"success":"true", "exists":"false", "message":"Email is available"})",
                        "application/json");
    });

    svr.listen("0.0.0.0", 8080);
    return 0;
}