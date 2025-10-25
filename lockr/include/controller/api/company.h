#ifndef LOCKR_CONTROLLER_API_COMPANY
#define LOCKR_CONTROLLER_API_COMPANY 1

#include "controller/api.h"

namespace lockr {
    void PutUserData(const httplib::Request& req, httplib::Response& res);
    void PostGetUserData(const httplib::Request& req, httplib::Response& res);

    void PostCreateKey(const httplib::Request& req, httplib::Response& res);
}

#endif