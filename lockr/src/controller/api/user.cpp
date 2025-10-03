#include "controller/api/user.h"

#include "service/user/create.h"
#include "service/user/exist.h"
#include "nlohmann/json.hpp"

using nlohmann::json;

void PostCreateUser(const Request& req, Response& res) {
    json body = json::parse(req.body, /*callback*/nullptr, /*allow_exceptions*/false);
    if (body.is_discarded()) {
        res.status = BadRequest_400;
        res.set_content(R"({"success": "false", "message": "Invalid JSON"})", "application/json");
    }

    string user = body["username"];
    string email = body["email"];
    string pass = body["password"];

    string msg;

    res.status = CreateUser(user, email, pass, msg);
    res.set_content(msg, "application/json");
}

void GetUsernameValid(const Request& req, Response& res) {
    if (!req.has_param("username")) {
        res.status = 400;
        res.set_content(R"({"success": false, "message": "Username required"})", "application/json");
        return;
    }
    string msg;

    res.status = CheckUsername(req.get_param_value("username"), msg);
    res.set_content(msg, "application/json");
}

void GetEmailValid(const Request& req, Response& res) {
    if (!req.has_param("email")) {
        res.status = 400;
        res.set_content(R"({"success": false, "message": "Email required"})", "application/json");
        return;
    }
    string msg;

    res.status = CheckEmail(req.get_param_value("email"), msg);
    res.set_content(msg, "application/json");
}