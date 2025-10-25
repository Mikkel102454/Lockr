#include "service/data.h"

#include "db.h"
#include "utils/json.h"

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

    bool GetData(const std::string& userId, const std::string& companyId, const std::vector<std::string> fields, std::string& outError, nlohmann::json& outJson) {
        bsoncxx::builder::basic::document proj;
        for (const auto& field : fields) {
            proj.append(bsoncxx::builder::basic::kvp(std::string("data.") + field, 1));
        }
        proj.append(bsoncxx::builder::basic::kvp("_id", 0));

        auto data = DB::getOne(
            "data",
            bsoncxx::builder::basic::make_document(
            bsoncxx::builder::basic::kvp("user_id", userId),
            bsoncxx::builder::basic::kvp("company_id", companyId)
            ),
            proj
        );

        const auto view = data->view();
        nlohmann::json doc_json = nlohmann::json::parse(bsoncxx::to_json(view));

        nlohmann::json jsonData;
        for (const auto& field : fields) {
            nlohmann::json val = GetJsonByPath(doc_json["data"], field);
            if (val.is_null()) {
                jsonData[field] = nullptr;
            } else {
                jsonData[field] = val;
            }
        }

        outJson = jsonData;
        return true;
    }

    bool GetAllData(const std::string& userId, const std::string& companyId, std::string& outError, nlohmann::json& outJson) {
        auto doc = DB::getOne("data", bsoncxx::builder::basic::make_document(
            bsoncxx::builder::basic::kvp("user_id", userId),
            bsoncxx::builder::basic::kvp("company_id", companyId)
            )
        );

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
        bsoncxx::oid oid;
        try {
            oid = bsoncxx::oid(userId);
        } catch (const std::exception&) {
            outError = "Invalid user ID format";
            return false;
        }

        if(!DB::Exists("user", bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("_id", oid)
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
        std::string err;
        GetAllData(userId, companyId, err, currentJson);
        if (currentJson == nullptr) {
            return ReplaceData(userId, json, companyId, outError);
        }
        currentJson.merge_patch(json);

        return ReplaceData(userId, currentJson, companyId, outError);
    }
}
