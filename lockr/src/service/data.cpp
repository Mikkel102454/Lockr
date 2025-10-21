#include "service/data.h"

#include "db.h"

#include "nlohmann/json.hpp"
#include <string>
#include <bsoncxx/json.hpp>

namespace lockr {
    static int maxData = 1024*1024; // 1MB

    bool RemoveAllData(const std::string& userId, const std::string& companyId, std::string& outError) {
        if(!DB::Exists("user", bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("_id", userId)
        ))) {
            outError = "User could not be found";
            return false;
        }

        return DB::DeleteOne("data", bsoncxx::builder::basic::make_document(
                  bsoncxx::builder::basic::kvp("user_id", userId),
                  bsoncxx::builder::basic::kvp("company_id", companyId)
                  ));
    }

    //TODO
    bool GetData(const std::string& userId, const std::string& companyId, const std::string fields[], std::string& outError, nlohmann::json& outJson) {

    }

    bool GetAllData(const std::string& userId, const std::string& companyId, std::string& outError, nlohmann::json& outJson) {
        auto doc = DB::getOne("data", bsoncxx::builder::basic::make_document(
                  bsoncxx::builder::basic::kvp("user_id", userId),
                  bsoncxx::builder::basic::kvp("company_id", companyId)
                  ));
        if (!doc) {
            outError = "User could not be found";
            outJson = nullptr;
            return false;
        }

        const auto dataView = doc->view()["data"].get_document().view();
        outJson = nlohmann::json::parse(bsoncxx::to_json(dataView));
        return true;
    }

    bool ReplaceData(const std::string& userId, const nlohmann::json& json, const std::string& companyId, std::string& outError) {
        if(!DB::Exists("user", bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("_id", userId)
        ))) {
            outError = "User could not be found";
            return false;
        }

        const bsoncxx::document::value jsonDoc = bsoncxx::from_json(json.dump());
        if (jsonDoc.view().length() > maxData){
            outError = "User's data limit has been reached";
            return false;
        };

        if (!DB::Exists("data", bsoncxx::builder::basic::make_document(
            bsoncxx::builder::basic::kvp("user_id", userId),
            bsoncxx::builder::basic::kvp("company_id", companyId)
            ))) {
            DB::Insert("data", bsoncxx::builder::basic::make_document(
            bsoncxx::builder::basic::kvp("user_id", userId),
            bsoncxx::builder::basic::kvp("company_id", companyId)
            ));
        }
        DB::UpdateOne("data", bsoncxx::builder::basic::make_document(
            bsoncxx::builder::basic::kvp("user_id", userId),
            bsoncxx::builder::basic::kvp("company_id", companyId)
            ),
            bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("data", jsonDoc.view())
            ));

        return true;
    }

    bool MergeData(const std::string& userId, const nlohmann::json& json, const std::string& companyId, std::string& outError) {
        nlohmann::json currentJson;
        if(!GetAllData(userId, companyId, outError, currentJson)) return false;
        if (currentJson == nullptr) {
            return ReplaceData(userId, json, companyId, outError);
        }
        currentJson.merge_patch(json);

        return ReplaceData(userId, currentJson, companyId, outError);
    }
}
