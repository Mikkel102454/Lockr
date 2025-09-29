#include "httplib.h"
#include "utils/configHandler.h"
#include <string>
#include "lockr/db.hpp"




using namespace httplib;

int main() {
    Server svr;

    //init db
    const char* env = std::getenv("MONGODB_URI");
    std::string uri = env ? env : "mongodb://127.0.0.1:37017";

    lockr::DB db{ uri };

    svr.Get("/test", [](const Request& req, Response& res) {
        try {
            char salt[BCRYPT_HASHSIZE];  // encoded salt
            char hash[BCRYPT_HASHSIZE];  // encoded hash ($2b$...)

            if (bcrypt_gensalt(12, salt) != 0) {
                res.status = 500;
                res.set_content("Failed to generate salt", "text/plain");
                return;
            }
            if (bcrypt_hashpw("secret", salt, hash) != 0) {
                res.status = 500;
                res.set_content("Failed to compute hash", "text/plain");
                return;
            }

            int rc = bcrypt_checkpw("secret", hash);
            bool match = (rc == 0);

            // Construct response with hash + match flag
            std::string body = std::string("hash: ") + hash +
                               "\nmatch: " + (match ? "true" : "false");

            res.status = 200;
            res.set_content(body, "text/plain");
        } catch (const std::exception& e) {
            res.status = 500;
            res.set_content(std::string("Config error: ") + e.what(), "text/plain");
        }
    });



    svr.listen("0.0.0.0", 8080);
    return 0;
}