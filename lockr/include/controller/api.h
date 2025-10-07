#ifndef LOCKR_CONTROLLER_API
#define LOCKR_CONTROLLER_API 1

#include "httplib.h"

namespace lockr {
    void InitEndpoint(httplib::Server &svr);
}

#endif