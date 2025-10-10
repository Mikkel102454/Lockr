#include "service/data.h"

#include "db.h"

#include "nlohmann/json.hpp"
#include <string>

namespace lockr {
    bool removeAll(const std::string& userId);
    bool get(const std::string& userId, nlohmann::json& outJson);
    bool replace(const std::string& userId, const nlohmann::json& json);
    bool merge(const std::string& userId, const nlohmann::json& json);

    bool removeAll(const std::string& userId) {
        return DB::DeleteOne("data", bsoncxx::builder::basic::make_document(
                  bsoncxx::builder::basic::kvp("userId", userId)
                  ));
    }

    bool get(const std::string& userId, nlohmann::json& outJson) {
        auto doc = DB::getOne("data", bsoncxx::builder::basic::make_document(
                  bsoncxx::builder::basic::kvp("userId", userId)
                  ));
        if (!doc) return false;

        auto v = doc->view();
        outJson = nlohmann::json::parse(std::string{ v["data"].get_string().value });
        return true;
    }

    bool replace(const std::string& userId, nlohmann::json& json) {
        return true;
    }

    bool merge(const std::string& userId, const nlohmann::json& json) {
        nlohmann::json currentJson;
        get(userId, currentJson);
        currentJson.merge_patch(json);

        replace(userId, currentJson);
        return true;
    }
}
