#include "httplib.h"
#include "utils/config.h"
#include "utils/env.h"

#include "controller/api.h"
#include <string>
#include "../include/db.h"
#include "../include/class/user.h"
#include "nlohmann/json.hpp"

using nlohmann::json;
using namespace httplib;

int main() {
    Server svr;

    //init db
    InitDotEnv();
    Config::initialize();
    DB::connect();

    InitEndpoint(svr);

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