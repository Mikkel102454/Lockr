#include "db.h"
#include "utils/hash.h"

#include "string"
#include <bsoncxx/builder/basic/document.hpp>

namespace lockr {
    void DeleteCompanyKey(std::string& userId, std::string& token) {
        std::string keyHashed = Hash(token);

        DB::DeleteOne("company_key", bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("user_id", userId),
                bsoncxx::builder::basic::kvp("key", keyHashed)
        ));
    }
}