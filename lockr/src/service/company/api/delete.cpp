#include "db.h"
#include "utils/hash.h"

#include "string"
#include <bsoncxx/builder/basic/document.hpp>

namespace lockr {
    void DeleteCompanyKey(std::string& userId, std::string& key) {
        std::string keyHashed = Hash(key);

        DB::DeleteOne("company_key", bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("user_id", userId),
                bsoncxx::builder::basic::kvp("key", keyHashed)
        ));
    }
}