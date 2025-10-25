#ifndef LOCKR_CONTROLLER_API__FRONTEND_PAGE
#define LOCKR_CONTROLLER_API__FRONTEND_PAGE 1

#include "controller/api.h"

namespace lockr {
    void GetPage(const httplib::Request& req, httplib::Response& res);
    void GetRootPage(const httplib::Request& req, httplib::Response& res);
}

#endif