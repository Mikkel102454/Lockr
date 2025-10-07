#ifndef LOCKR_CONTROLLER_API_USER
#define LOCKR_CONTROLLER_API_USER 1

#include "controller/api.h"

#include "httplib.h"

namespace lockr {
    void PostCreateUser(const httplib::Request& req, httplib::Response& res);
    void GetUsernameValid(const httplib::Request& req, httplib::Response& res);
    void GetEmailValid(const httplib::Request& req, httplib::Response& res);

    void PostLogin(const httplib::Request& req, httplib::Response& res);
}

#endif