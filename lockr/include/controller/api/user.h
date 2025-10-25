#ifndef LOCKR_CONTROLLER_API_USER
#define LOCKR_CONTROLLER_API_USER 1

#include "controller/api.h"

namespace lockr {
    void PostCreateUser(const httplib::Request& req, httplib::Response& res);
    void PostUsernameValid(const httplib::Request& req, httplib::Response& res);
    void PostEmailValid(const httplib::Request& req, httplib::Response& res);

    void PostLogin(const httplib::Request& req, httplib::Response& res);
}

#endif