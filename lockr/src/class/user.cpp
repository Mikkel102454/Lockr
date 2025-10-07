#include "class/user.h"

#include "db.h"
#include "utils/env.h"

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/types.hpp>

namespace lockr {
    int User::HashPassword(const std::string &password, std::string &outHash) {
        char salt[BCRYPT_HASHSIZE];
        char hash[BCRYPT_HASHSIZE];

        if (bcrypt_gensalt(12, salt) != 0) {
            return 1;
        }
        if (bcrypt_hashpw(password.c_str(), salt, hash) != 0) {
            return 2;
        }

        outHash.assign(hash);
        return 0;
    }

    bool User::Authenticate(const std::string &email, const std::string &password) {
        auto doc = DB::getOne("user",
                              bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("email", email)));
        if (!doc) return false;

        auto v = doc->view();
        auto pwEl = v["password"];
        if (!pwEl || pwEl.type() != bsoncxx::type::k_string) return false;

        const std::string storedHash = std::string{ pwEl.get_string().value };

        const int rc = bcrypt_checkpw(password.c_str(), storedHash.c_str());
        if (rc == 0)  return true;

        return false;
    }

    std::string User::GetUserId(const std::string &email) {
        auto doc = DB::getOne("user", bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("email", email)
        ));
        if (!doc) return "";

        auto v = doc->view();
        return v["_id"].get_oid().value.to_string();
    }

    bool User::UsernameExist(const std::string& username) {
        return DB::Exists("user", bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("username", username)));
    }

    bool User::EmailExist(const std::string& email) {
        return DB::Exists("user", bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("email", email)));
    }

    int User::save() {
        if(UsernameExist(m_username)) return 1;
        if(EmailExist(m_email)) return 2;

        auto doc = bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("username", m_username),
                bsoncxx::builder::basic::kvp("email", m_email),
                bsoncxx::builder::basic::kvp("password", m_password),
                bsoncxx::builder::basic::kvp("companies", ""),
                bsoncxx::builder::basic::kvp("data", "")
        );

        std::string dbError;
        int ec = DB::Insert("user", std::move(doc), &dbError);

        if(ec == 0) return 0;
        if(ec == 1) return 1;
        if(ec == 2) return 2;

        return 3;
    }
}
