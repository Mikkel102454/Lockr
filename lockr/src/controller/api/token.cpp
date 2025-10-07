#include "controller/api/token.h"
#include "service/auth/access.h"
#include "service/auth/refresh.h"
#include "nlohmann/json.hpp"

namespace lockr {
    void PostRefreshToken(const httplib::Request& req, httplib::Response& res) {

        if(!ValidateRefreshToken(req.get_param_value("refreshToken"))) {
            nlohmann::json j = {
                    {"success", false},
                    {"message", "Refresh token is invalid."}
            };
            res.set_content(j.dump(), "application/json");
            res.status = httplib::Unauthorized_401;
            return;
        }

        std::string accessToken;
        std::string userId = GetIdFromRefreshToken(req.get_param_value("refreshToken"));

        CreateNewAccessToken(accessToken, userId);
        InvalidateRefreshToken(req.get_param_value("refreshToken"));
        if(userId.empty()) {
            nlohmann::json j = {
                    {"success", false},
                    {"message", "User not found."}
            };
            res.set_content(j.dump(), "application/json");
            res.status = httplib::NotFound_404;
            return;
        }

        std::string refreshToken;
        GenerateRefreshToken(refreshToken, userId);

        nlohmann::json j = {
                {"success", true},
                {"message", "Successfully created new access token."},
                {"accessToken", accessToken},
                {"refreshToken", refreshToken}
        };

        res.set_content(j.dump(), "application/json");
    }

    void PostValidateRefreshToken(const httplib::Request& req, httplib::Response& res) {
        if (!ValidateRefreshToken(req.get_param_value("refreshToken"))) {
            nlohmann::json j = {
                    {"success", false},
                    {"message", "Refresh token is invalid."},
            };

            res.status = httplib::Unauthorized_401;
            res.set_content(j.dump(), "application/json");
            return;
        }
        nlohmann::json j = {
                {"success", true},
                {"message", "Refresh token is valid."}
        };

        res.status = httplib::OK_200;
        res.set_content(j.dump(), "application/json");
    }

    void PostValidateAccessToken(const httplib::Request& req, httplib::Response& res) {
        if (!ValidateAccessToken(req.get_param_value("accessToken"))) {
            nlohmann::json j = {
                    {"success", false},
                    {"message", "Access token is invalid."},
            };

            res.status = httplib::Unauthorized_401;
            res.set_content(j.dump(), "application/json");
            return;
        }
        nlohmann::json j = {
                {"success", true},
                {"message", "Access token is valid."}
        };

        res.status = httplib::OK_200;
        res.set_content(j.dump(), "application/json");
    }
}