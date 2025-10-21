#ifndef LOCKR_SERVICE_COMPANY_API_VERIFY
#define LOCKR_SERVICE_COMPANY_API_VERIFY 1

#include <string>

namespace lockr {
    bool ValidateCompanyKey(const std::string& token, std::string& outName);
}

#endif
