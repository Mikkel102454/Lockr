#include "db.h"
#include "utils/hash.h"

#include "string"
#include <bsoncxx/builder/basic/document.hpp>

namespace lockr {
    bool ValidateCompanyKey(const std::string& key, std::string& outId) {
        std::string keyHashed = Hash(key);

        if (!DB::Exists("company_key", bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("key", keyHashed)
        ))) {
            return false;
        }

        const auto doc = DB::getOne("company_key", bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("key", keyHashed)
        ));
        bsoncxx::document::view view = doc->view();
        outId = std::string{ view["_id"].get_oid().value.to_string() };
        return true;
    }

    bool ValidateCompanyToken(const std::string& token, std::string& outId, std::string& outRedirect) {

        if (!DB::Exists("company_token", bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("token", token)
        ))) {
            return false;
        }

        const auto doc = DB::getOne("company_token", bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("token", token)
        ));
        bsoncxx::document::view view = doc->view();
        outId = std::string{ view["company_id"].get_string().value };
        outRedirect = std::string{ view["redirect"].get_string().value };
        return true;
    }
}