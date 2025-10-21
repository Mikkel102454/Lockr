#include "controller/api/company.h"

#include <bsoncxx/builder/basic/array.hpp>

#include "service/data.h"
#include "service/company/api/verify.h"
#include "service/auth/access.h"

#include "nlohmann/json.hpp"
#include "service/company/api/create.h"

namespace lockr {
    void PutUserData(const httplib::Request& req, httplib::Response& res) {
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
        if (!ValidateCompanyKey(body["token"], companyId)) {
            nlohmann::json j = {
                    {"message", "Invalid company token."}
            };
            res.set_content(j.dump(), "application/json");
            res.status = httplib::Unauthorized_401;
            return;
        }

        std::string errorMessage;
        if (body.value("replace", false)) {
             ReplaceData(body["userId"],
                                  body["data"], companyId, errorMessage);
        }else {
             MergeData(body["userId"],
                                body["data"], companyId, errorMessage);
        }
        if (!errorMessage.empty()) {
            nlohmann::json j = {
                    {"message", errorMessage}
            };
            res.set_content(j.dump(), "application/json");
            res.status = httplib::UnprocessableContent_422;
            return;
        }

        nlohmann::json j = {
                {"message", "User data has been successfully uploaded"}
        };
        res.set_content(j.dump(), "application/json");
        res.status = httplib::Created_201;
    }

    void GetUserData(const httplib::Request& req, httplib::Response& res) {
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
        if (!ValidateCompanyKey(body["token"], companyId)) {
            nlohmann::json j = {
                    {"message", "Invalid company token."}
            };
            res.set_content(j.dump(), "application/json");
            res.status = httplib::Unauthorized_401;
            return;
        }

        std::string errorMessage;
        nlohmann::json outData;
        if (body.contains("fields") && body["fields"].is_array()) {
            std::string* fields = new std::string[body["fields"].size()];

            try {
                size_t i = 0;
                for (const auto& f : body["fields"]) {
                    if (f.is_string()) {
                        fields[i++] = f.get<std::string>();
                    }
                }
                GetData(body["userId"], companyId, fields, errorMessage, outData);
                delete[] fields;
            } catch (std::exception& e){
                delete[] fields;
            }
        }else {
            GetAllData(body["userId"], companyId, errorMessage, outData);
        }

        if (!errorMessage.empty()) {
            nlohmann::json j = {
                    {"message", errorMessage}
            };
            res.set_content(j.dump(), "application/json");
            res.status = httplib::UnprocessableContent_422;
            return;
        }

        nlohmann::json j = {
                {"userId", body["userId"]},
                {"data", outData}
        };
        res.set_content(j.dump(), "application/json");
        res.status = httplib::OK_200;
    }

    void PostCreateKey(const httplib::Request& req, httplib::Response& res) {
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

        std::string userId;
        if (!ValidateAccessToken(body["accessToken"], userId)) {
            nlohmann::json j = {
                {"message", "Invalid access token."}
            };
            res.set_content(j.dump(), "application/json");
            res.status = httplib::Unauthorized_401;
            return;
        }

        std::string outKey;
        CreateCompanyKey(outKey, userId, body["newName"]);
        if (outKey.length() == 0) {
            nlohmann::json j = {
                {"message", "Internal server error"}
            };
            res.set_content(j.dump(), "application/json");
            res.status = httplib::InternalServerError_500;
            return;
        }
        nlohmann::json j = {
            {"token", outKey}
        };
        res.set_content(j.dump(), "application/json");
        res.status = httplib::Created_201;
    }


    void PostDeleteKey(const httplib::Request& req, httplib::Response& res) {

    }
    void PostGetKeys(const httplib::Request& req, httplib::Response& res) {

    }
}
