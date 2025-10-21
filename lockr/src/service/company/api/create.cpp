#include "db.h"
#include "utils/hash.h"

#include "string"
#include <bsoncxx/builder/basic/document.hpp>
#include <openssl/rand.h>

#include "utils/string.h"

namespace lockr {
    void CreateCompanyKey(std::string& outToken, const std::string& userId,
                                    const std::string& name) {
        unsigned char tokenChar[32];
        RAND_bytes(tokenChar, 32);

        const std::string token = Base64urlEncode(tokenChar, 32);
        outToken = token;
        std::string keyHashed = Hash(token);

        DB::Insert("company_key", bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("user_id", userId),
                bsoncxx::builder::basic::kvp("name", name),
                bsoncxx::builder::basic::kvp("key", keyHashed)
                ));
    }
}
