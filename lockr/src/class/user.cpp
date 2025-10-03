#include "../../include/class/user.h"

#include "../../include/db.h"

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <iostream>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

int User::hashPassword(const std::string& password, std::string& outHash) {
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

int User::save() {
    if(usernameExist(u_username)) return 1;
    if(emailExist(u_email)) return 2;

    auto doc = make_document(
        kvp("username", u_username),
        kvp("email", u_email),
        kvp("password", u_password),
        kvp("companies", ""),
        kvp("data", "")
    );

    std::string dbError;
    int ec = DB::insert("user", std::move(doc), &dbError);

    if(ec == 0) return 0;
    if(ec == 1) return 1;
    if(ec == 2) return 2;

    std::cout << dbError;
    return 3;
}

int User::usernameExist(std::string username) {
    if(!DB::exists("user", make_document(kvp("username", username)))){
        return 0;
    }
    return 1;
}

int User::emailExist(std::string email) {
    if(!DB::exists("user", make_document(kvp("email", email)))){
        return 0;
    }
    return 1;
}