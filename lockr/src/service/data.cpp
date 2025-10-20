#include "service/data.h"

#include "db.h"

#include "nlohmann/json.hpp"
#include <string>

namespace lockr {
    // TODO make api key
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

    // TODO convert to bson and then input that into mongodb
    bool replace(const std::string& userId, nlohmann::json& json) {
        if (json.dump().size() > 1024 * 1024) return false;

        DB::ReplaceOne("data", bsoncxx::builder::basic::make_document(
            bsoncxx::builder::basic::kvp("user_id", userId)
            ),
        bsoncxx::builder::basic::make_document(
            bsoncxx::builder::basic::kvp("data", json.dump())
            ));

        return true;
    }

    bool merge(const std::string& userId, const nlohmann::json& json) {
        if (json.dump().size() > 1024 * 1024) return false;

        nlohmann::json currentJson;
        get(userId, currentJson);
        currentJson.merge_patch(json);

        replace(userId, currentJson);
        return true;
    }
}
