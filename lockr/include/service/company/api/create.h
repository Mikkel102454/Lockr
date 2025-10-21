#ifndef LOCKR_SERVICE_COMPANY_API_CREATE
#define LOCKR_SERVICE_COMPANY_API_CREATE 1

#include <string>

namespace lockr {
    void CreateCompanyKey(std::string& outToken, const std::string& userId,
                                    const std::string& name);
}

#endif
