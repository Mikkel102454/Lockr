#include "db.h"
#include "utils/hash.h"

#include "string"
#include <bsoncxx/builder/basic/document.hpp>
#include <openssl/rand.h>

namespace lockr {
    std::string CreateCompanyKey(std::string& outKey, const std::string& userId,
                                    const std::string& name) {
        unsigned char token[32];
        RAND_bytes(token, 32);

        outKey = reinterpret_cast<char*>(token);
        std::string keyHashed = Hash(reinterpret_cast<char*>(token));

        return DB::Insert("company_key", bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("user_id", userId),
                bsoncxx::builder::basic::kvp("name", name),
                bsoncxx::builder::basic::kvp("key", keyHashed)
                ));
    }
}