#include "controller/api/user.h"

#include "service/user/create.h"
#include "nlohmann/json.hpp"

using nlohmann::json;

void PostCreateUser(const Request& req, Response& res) {
    json body = json::parse(req.body, /*callback*/nullptr, /*allow_exceptions*/false);
    if (body.is_discarded()) {
        res.status = BadRequest_400;
        res.set_content(R"({"success":"false", "message":"Invalid JSON"})", "application/json");
    }

    std::string user = body["username"];
    std::string email = body["email"];
    std::string pass = body["password"];

    std::string msg;

    res.status = CreateUser(user, email, pass, msg);
    res.set_content(res.body, msg);
}