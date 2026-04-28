#ifndef LOCKR_SERVICE_COMPANY_API_VERIFY
#define LOCKR_SERVICE_COMPANY_API_VERIFY 1

#include <string>

namespace lockr {
    bool ValidateCompanyKey(const std::string& key, std::string& outId);
    bool ValidateCompanyToken(const std::string& token, std::string& outId, std::string& outDomain  );
}

#endif
