#include "controller/api/company.h"

#include <bsoncxx/builder/basic/array.hpp>

#include "service/data.h"
#include "service/company/api/verify.h"
#include "service/auth/access.h"

#include "nlohmann/json.hpp"
#include "service/company/api/create.h"

namespace lockr {
    void PutUserData(const httplib::Request& req, httplib::Response& res) {
        try {
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

            std::string companyName;
            if (!ValidateCompanyKey(body["token"], companyName)) {
                nlohmann::json j = {
                    {"message", "Invalid company token."}
                };
                res.set_content(j.dump(), "application/json");
                res.status = httplib::Unauthorized_401;
                return;
            }

            bool success = false;
            if (body.value("replace", false)) {
                success = ReplaceData(body["userId"],
                    body["data"], companyName);
            }else {
                success = MergeData(body["userId"],
                    body["data"], companyName);
            }
            if (!success) {
                nlohmann::json j = {
                    {"message", "User data limit has been reached."}
                };
                res.set_content(j.dump(), "application/json");
                res.status = httplib::InsufficientStorage_507;
                return;
            }

            res.status = httplib::Created_201;
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }

    void PostCreateKey(const httplib::Request& req, httplib::Response& res) {
        std::string userId;
        if (!ValidateAccessToken(req.get_param_value("accessToken"), userId)) {
            nlohmann::json j = {
                {"message", "Invalid access token."}
            };
            res.set_content(j.dump(), "application/json");
            res.status = httplib::Unauthorized_401;
            return;
        }

        std::string outKey;
        CreateCompanyKey(outKey, userId, req.get_param_value("newName"));
        if (outKey.length() == 0) {
            nlohmann::json j = {
                {"message", "Internal server error"}
            };
            res.set_content(j.dump(), "application/json");
            res.status = httplib::InternalServerError_500;
            return;
        }
        std::cout << outKey << std::endl;
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
