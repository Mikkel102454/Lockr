//
// Created by TRP Solutions ApS on 07-10-2025.
//

#include "controller/api/token.h"
#include "service/auth/access.h"
#include "service/auth/refresh.h"
#include "nlohmann/json.hpp"

using nlohmann::json;

void PostRefreshToken(const Request& req, Response& res) {
    string accessToken;
    res.status = CreateNewAccessToken(req.get_param_value("refreshToken"), accessToken);

    if (res.status != 200) {
        json j = {
            {"success", true},
            {"message", "No valid refresh token"}
        };
        res.set_content(j.dump(), "application/json");
        return;
    }

    string refreshToken;
    string userId = GetIdFromRefreshToken(req.get_param_value("refreshToken"));

    InvalidateRefreshToken(req.get_param_value("refreshToken"));
    GenerateRefreshToken(refreshToken, userId);

    json j = {
        {"success", true},
        {"message", "Login successful"},
        {"accessToken", accessToken},
        {"refreshToken", refreshToken}
    };

    res.set_content(j.dump(), "application/json");
}

void PostValidateRefreshToken(const Request& req, Response& res) {
    if (!ValidateRefreshToken(req.get_param_value("refreshToken"))) {
        json j = {
            {"success", false},
            {"message", "Refresh token is invalid"},
        };

        res.status = 409;
        res.set_content(j.dump(), "application/json");
    }
    json j = {
        {"success", true},
        {"message", "Refresh token is valid"}
    };

    res.status = 200;
    res.set_content(j.dump(), "application/json");
}

void PostValidateAccessToken(const Request& req, Response& res) {
    if (!ValidateAccessToken(req.get_param_value("accessToken"))) {
        json j = {
            {"success", false},
            {"message", "Access token is invalid"},
        };

        res.status = 409;
        res.set_content(j.dump(), "application/json");
    }
    json j = {
        {"success", true},
        {"message", "Access token is valid"}
    };

    res.status = 200;
    res.set_content(j.dump(), "application/json");
}