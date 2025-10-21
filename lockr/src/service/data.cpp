#include "service/data.h"

#include "db.h"

#include "nlohmann/json.hpp"
#include <string>

namespace lockr {
    static int maxData = 1024*1024; // 1MB

    bool RemoveAllData(const std::string& userId, const std::string& companyId) {
        return DB::DeleteOne("data", bsoncxx::builder::basic::make_document(
                  bsoncxx::builder::basic::kvp("user_id", userId),
                  bsoncxx::builder::basic::kvp("company_id", companyId)
                  ));
    }

    bool GetData(const std::string& userId, nlohmann::json& outJson, const std::string& companyId) {
        auto doc = DB::getOne("data", bsoncxx::builder::basic::make_document(
                  bsoncxx::builder::basic::kvp("user_id", userId),
                  bsoncxx::builder::basic::kvp("company_id", companyId)
                  ));
        if (!doc) return false;

        auto v = doc->view();
        outJson = nlohmann::json::parse(std::string{ v["data"].get_string().value });
        return true;
    }

    bool ReplaceData(const std::string& userId, const nlohmann::json& json, const std::string& companyId) {
        const auto bson = nlohmann::json::to_bson(json);
        if (bson.size() > maxData) return false;

        DB::UpdateOne("data", bsoncxx::builder::basic::make_document(
            bsoncxx::builder::basic::kvp("user_id", userId),
            bsoncxx::builder::basic::kvp("company_id", companyId)
            ),
            bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("data",  bsoncxx::types::b_binary{
                bsoncxx::binary_sub_type::k_binary,
                static_cast<uint32_t>(bson.size()),
                bson.data()
                })
            ));

        return true;
    }

    bool MergeData(const std::string& userId, const nlohmann::json& json, const std::string& companyId) {
            nlohmann::json currentJson;
            GetData(userId, currentJson, companyId);
            currentJson.merge_patch(json);

            return ReplaceData(userId, currentJson, companyId);
    }
}
