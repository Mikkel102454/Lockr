#include "controller/api/token.h"
#include "service/auth/access.h"
#include "service/auth/refresh.h"
#include "service/company/api/verify.h"

#include "nlohmann/json.hpp"

namespace lockr {
    void PostRefreshToken(const httplib::Request& req, httplib::Response& res) {
        nlohmann::json body = nlohmann::json::parse(req.body, nullptr, false);
        if (body.is_discarded()) {
            nlohmann::json j = {
                {"success", false},
                {"message", "Invalid JSON."}
            };
            res.set_content(j.dump(), "application/json");
            res.status = httplib::BadRequest_400;
            return;
        }

        std::string companyId;
        if(!ValidateCompanyKey(body["companyToken"], companyId)){
            nlohmann::json j = {
                    {"success", false},
                    {"message", "Invalid company token."}
            };
            res.set_content(j.dump(), "application/json");
            res.status = httplib::Unauthorized_401;
            return;
        }

        if(!ValidateRefreshToken(body["refreshToken"], companyId)) {
            nlohmann::json j = {
                    {"success", false},
                    {"message", "Refresh token is invalid."}
            };
            res.set_content(j.dump(), "application/json");
            res.status = httplib::Unauthorized_401;
            return;
        }

        std::string accessToken;
        std::string userId = GetIdFromRefreshToken(body["refreshToken"], companyId);

        CreateNewAccessToken(accessToken, companyId, userId);
        InvalidateRefreshToken(body["refreshToken"], companyId);
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
        GenerateRefreshToken(refreshToken, userId, companyId);

        nlohmann::json j = {
                {"success", true},
                {"message", "Successfully created new access token."},
                {"accessToken", accessToken},
                {"refreshToken", refreshToken}
        };

        res.set_content(j.dump(), "application/json");
    }

    void PostValidateRefreshToken(const httplib::Request& req, httplib::Response& res) {
        nlohmann::json body = nlohmann::json::parse(req.body, nullptr, false);
        if (body.is_discarded()) {
            nlohmann::json j = {
                {"success", false},
                {"message", "Invalid JSON."}
            };
            res.set_content(j.dump(), "application/json");
            res.status = httplib::BadRequest_400;
            return;
        }

        std::string companyId;
        if(!ValidateCompanyKey(body["companyToken"], companyId)){
            nlohmann::json j = {
                    {"success", false},
                    {"message", "Invalid company token."}
            };
            res.set_content(j.dump(), "application/json");
            res.status = httplib::Unauthorized_401;
            return;
        }

        if (!ValidateRefreshToken(body["refreshToken"], companyId)) {
            nlohmann::json j = {
                    {"success", false},
                    {"message", "Refresh token is invalid."}
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
        try{
            nlohmann::json body = nlohmann::json::parse(req.body, nullptr, false);
            if (body.is_discarded()) {
                nlohmann::json j = {
                    {"success", false},
                    {"message", "Invalid JSON."}
                };
                res.set_content(j.dump(), "application/json");
                res.status = httplib::BadRequest_400;
                return;
            }

            std::string companyId;
            if(!ValidateCompanyKey(body["companyToken"], companyId)){
                nlohmann::json j = {
                        {"success", false},
                        {"message", "Invalid company token."}
                };
                res.set_content(j.dump(), "application/json");
                res.status = httplib::Unauthorized_401;
                return;
            }

            if (!ValidateAccessToken(body["accessToken"], companyId)) {
                nlohmann::json j = {
                        {"success", false},
                        {"message", "Access token is invalid."}
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
        }catch (std::exception &e){
            std::cout << e.what();
        }
    }
}