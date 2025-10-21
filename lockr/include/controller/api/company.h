#ifndef LOCKR_CONTROLLER_API_COMPANY
#define LOCKR_CONTROLLER_API_COMPANY 1

#include "controller/api.h"

#include "httplib.h"

namespace lockr {
    void PutUserData(const httplib::Request& req, httplib::Response& res);
    void GetUserData(const httplib::Request& req, httplib::Response& res);

    void PostCreateKey(const httplib::Request& req, httplib::Response& res);
}

#endif