//
// Created by TRP Solutions ApS on 07-10-2025.
//

#include "controller/api/token.h"
#include "service/auth/access.h"

void PostRefreshToken(const Request& req, Response& res) {
    string token;

    res.status = CreateNewAccessToken(req.get_param_value("username"), token);
    res.set_content(token, "application/json");
}