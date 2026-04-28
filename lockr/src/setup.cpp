#include "db.h"
#include "service/user/create.h"
#include "service/company/api/create.h"
#include "utils/logger.h"

int setup() {
    if (!lockr::DB::Exists("user", bsoncxx::builder::basic::make_document(
            bsoncxx::builder::basic::kvp("admin", true)
    ))) {
        nlohmann::json msg;
        lockr::CreateUser("admin", "admin@admin.dk", "admin", true, msg);
        lockr::Log(lockr::INFO, "Created default admin account");
    }

    const auto doc = lockr::DB::getOne("user", bsoncxx::builder::basic::make_document(
            bsoncxx::builder::basic::kvp("username", "admin")
    ));

    bsoncxx::document::view view = doc->view();
    std::string id = std::string{view["_id"].get_oid().value.to_string()};

    if (!lockr::DB::Exists("company_key", bsoncxx::builder::basic::make_document(
            bsoncxx::builder::basic::kvp("user_id", id)
    ))) {
        nlohmann::json msg;
        std::string outKey;
        std::string outToken;
        std::string outId;
        lockr::CreateCompanyKey(outKey, outId, id, "lockr");

        lockr::CreateCompanyToken(outToken, id, outId, "lockr.com");
        lockr::Log(lockr::INFO, "Created default company");
        lockr::Log(lockr::INFO, "-----------------------");
        lockr::Log(lockr::INFO, "Key: " + outKey);
        lockr::Log(lockr::INFO, "Token: " + outToken);
        lockr::Log(lockr::INFO, "Id: " + outId);
        lockr::Log(lockr::INFO, "-----------------------");

    }
    return 0;
}