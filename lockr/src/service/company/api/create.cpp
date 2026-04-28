#include "db.h"
#include "utils/hash.h"

#include "string"
#include <bsoncxx/builder/basic/document.hpp>
#include <openssl/rand.h>

#include "utils/string.h"

namespace lockr {
    void CreateCompanyKey(std::string& outKey, std::string& outId,
                            const std::string& userId, const std::string& name) {
        unsigned char keyChar[32];
        RAND_bytes(keyChar, 32);

        const std::string key = Base64urlEncode(keyChar, 32);
        outKey = key;
        std::string keyHashed = Hash(key);
        outId = DB::Insert("company_key", bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("user_id", userId),
                bsoncxx::builder::basic::kvp("name", name),
                bsoncxx::builder::basic::kvp("key", keyHashed)
                ));
    }

    void CreateCompanyToken(std::string& outToken, const std::string& userId,
                                const std::string& compId, const std::string& domain) {
        unsigned char tokenChar[32];
        RAND_bytes(tokenChar, 32);

        const std::string token = Base64urlEncode(tokenChar, 32);
        outToken = token;

        DB::Insert( "company_token", bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("user_id", userId),
                bsoncxx::builder::basic::kvp("company_id", compId),
                bsoncxx::builder::basic::kvp("token", token),
                bsoncxx::builder::basic::kvp("domain", domain)
                ));
    }
}
