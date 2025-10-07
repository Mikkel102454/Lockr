#ifndef LOCKR_CONTROLLER_API_TOKEN
#define LOCKR_CONTROLLER_API_TOKEN 1

#include "controller/api.h"

#include "httplib.h"

namespace lockr {
    void PostRefreshToken(const httplib::Request& req, httplib::Response& res);


    void PostValidateRefreshToken(const httplib::Request& req, httplib::Response& res);
    void PostValidateAccessToken(const httplib::Request& req, httplib::Response& res);
}

#endif