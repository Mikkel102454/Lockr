#include "controller/api/user.h"

#include "service/user/create.h"
#include "service/user/exist.h"
#include "service/user/login.h"
#include "nlohmann/json.hpp"

namespace lockr {
    void PostCreateUser(const httplib::Request& req, httplib::Response& res) {
        nlohmann::json body = nlohmann::json::parse(req.body, /*callback*/nullptr, /*allow_exceptions*/false);
        if (body.is_discarded()) {
            nlohmann::json j = {
                    {"success", false},
                    {"message", "Invalid JSON."}
            };
            res.set_content(j.dump(), "application/json");
            res.status = httplib::BadRequest_400;
            return;
        }

        std::string user = body["username"];
        std::string email = body["email"];
        std::string pass = body["password"];

        nlohmann::json msg;

        res.status = CreateUser(user, email, pass, msg);
        res.set_content(msg.dump(), "application/json");
    }

    void GetUsernameValid(const httplib::Request& req, httplib::Response& res) {
        if (!req.has_param("username")) {
            nlohmann::json j = {
                    {"success", false},
                    {"message", "Username required."}
            };
            res.set_content(j.dump(), "application/json");
            res.status = httplib::BadRequest_400;
            return;
        }
        nlohmann::json msg;

        res.status = CheckUsername(req.get_param_value("username"), msg);
        res.set_content(msg.dump(), "application/json");
    }

    void GetEmailValid(const httplib::Request& req, httplib::Response& res) {
        if (!req.has_param("email")) {
            nlohmann::json j = {
                    {"success", false},
                    {"message", "Email required."}
            };
            res.set_content(j.dump(), "application/json");
            res.status = httplib::BadRequest_400;
            return;
        }
        nlohmann::json msg;

        res.status = CheckEmail(req.get_param_value("email"), msg);
        res.set_content(msg.dump(), "application/json");
    }

    void PostLogin(const httplib::Request& req, httplib::Response& res) {
        try {
            if (!req.has_param("email")) {
                nlohmann::json j = {
                        {"success", false},
                        {"message", "Email required."}
                };
                res.set_content(j.dump(), "application/json");
                res.status = httplib::BadRequest_400;
                return;
            }
            if (!req.has_param("password")) {
                nlohmann::json j = {
                        {"success", false},
                        {"message", "Password required."}
                };
                res.set_content(j.dump(), "application/json");
                res.status = httplib::BadRequest_400;
                return;
            }

            std::string refreshToken;

            if (!Login(req.get_param_value("email"), req.get_param_value("password"), refreshToken)) {
                nlohmann::json j = {
                        {"success", false},
                        {"message", "Email or password is incorrect."}
                };
                res.set_content(j.dump(), "application/json");
                res.status = httplib::Unauthorized_401;
                return;
            }

            nlohmann::json j = {
                    {"success", true},
                    {"message", "Login successful."},
                    {"refreshToken", refreshToken}
            };
            res.set_content(j.dump(), "application/json");
            res.status = httplib::OK_200;
        }catch (std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    }
}