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
        auto view = DB::getOne("company_key", bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("key", keyHashed)
        ))->view();
        outId = std::string{ view["_id"].get_oid().value.to_string() };
        return true;
    }
}