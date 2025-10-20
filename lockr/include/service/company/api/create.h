#ifndef LOCKR_SERVICE_COMPANY_API_CREATE
#define LOCKR_SERVICE_COMPANY_API_CREATE 1

#include <string>

namespace lockr {
    std::string CreateCompanyKey(std::string& userId, std::string& name);
}

#endif
