#ifndef LOCKR_CONTROLLER_API__FRONTEND_RESOURCE
#define LOCKR_CONTROLLER_API__FRONTEND_RESOURCE 1

#include "controller/api.h"

#include "httplib.h"

namespace lockr {
    void GetStyle(const httplib::Request& req, httplib::Response& res);
    void GetCode(const httplib::Request& req, httplib::Response& res);
}

#endif