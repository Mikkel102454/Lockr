#ifndef LOCKR_SERVICE_DATA
#define LOCKR_SERVICE_DATA 1

#include "nlohmann/json.hpp"
#include <string>

namespace lockr {
    bool RemoveAllData(const std::string& userId, const std::string& companyId, std::string& outError);
    bool GetData(const std::string& userId, const std::string& companyId, const std::vector<std::string> fields, std::string& outError, nlohmann::json& outJson);
    bool GetAllData(const std::string& userId, const std::string& companyId, std::string& outError, nlohmann::json& outJson);
    bool ReplaceData(const std::string& userId, const nlohmann::json& json, const std::string& companyId, std::string& outError);
    bool MergeData(const std::string& userId, const nlohmann::json& json, const std::string& companyId, std::string& outError);
}

#endif