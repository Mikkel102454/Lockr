#ifndef LOCKR_UTILS_JSON
#define LOCKR_UTILS_JSON 1

#include <nlohmann/json.hpp>
#include <string>

namespace lockr {
    nlohmann::json GetJsonByPath(const nlohmann::json& root, const std::string& dot_path);
}

#endif