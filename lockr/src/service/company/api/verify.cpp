#include "db.h"
#include "utils/hash.h"

#include "string"
#include <bsoncxx/builder/basic/document.hpp>

namespace lockr {
    bool VerifyCompanyKey(std::string& key) {
        std::string keyHashed = Hash(key);

        return DB::Exists("company_key", bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("key", keyHashed)
        ));
    }
}