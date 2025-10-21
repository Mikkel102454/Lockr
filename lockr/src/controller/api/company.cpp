#include "controller/api/company.h"

#include "service/data.h"
#include "service/company/api/verify.h"
#include "service/auth/access.h"

#include "nlohmann/json.hpp"
#include "service/company/api/create.h"

namespace lockr {
    void PutUserData(const httplib::Request& req, httplib::Response& res) {
        std::string companyName;
        if (!ValidateCompanyKey(req.get_param_value("token"), companyName)) {
            nlohmann::json j = {
                {"message", "Invalid company token."}
            };
            res.set_content(j.dump(), "application/json");
            res.status = httplib::Unauthorized_401;
            return;
        }

        bool success = false;
        if (req.get_param_value("replace").data()) {
            success = ReplaceData(req.get_param_value("userId"),
                req.get_param_value("data"), companyName);
        }else {
            success = MergeData(req.get_param_value("userId"),
                req.get_param_value("data"), companyName);
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
        std::string out = CreateCompanyKey(outKey, userId, req.get_param_value("newName"));
        if (out.length() == 0) {
            // 500
        }
    }
    void PostDeleteKey(const httplib::Request& req, httplib::Response& res) {

    }
    void PostGetKeys(const httplib::Request& req, httplib::Response& res) {

    }
}
