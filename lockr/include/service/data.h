#ifndef LOCKR_SERVICE_DATA
#define LOCKR_SERVICE_DATA 1

#include "nlohmann/json.hpp"
#include <string>

namespace lockr {
    bool RemoveAllData(const std::string& userId, const std::string& companyId);
    bool GetData(const std::string& userId, nlohmann::json& outJson, const std::string& companyId, const std::string fields[]);
    bool GetAllData(const std::string& userId, nlohmann::json& outJson, const std::string& companyId);
    bool ReplaceData(const std::string& userId, const nlohmann::json& json, const std::string& companyId);
    bool MergeData(const std::string& userId, const nlohmann::json& json, const std::string& companyId);
}

#endif