#include "class/user.h"

#include "db.h"
#include "utils/env.h"

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/types.hpp>
#include <iostream>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>
#include <cstring>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using bsoncxx::type;

int User::hashPassword(const string& password, string& outHash) {
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

    string dbError;
    int ec = DB::insert("user", move(doc), &dbError);

    if(ec == 0) return 0;
    if(ec == 1) return 1;
    if(ec == 2) return 2;

    cout << dbError;
    return 3;
}

bool User::Authenticate(const string &email, const string &password) {
    auto doc = DB::getOne("user",
        make_document(kvp("email", email)));
    if (!doc) return false;

    auto v = doc->view();
    auto pwEl = v["password"];
    if (!pwEl || pwEl.type() != type::k_string) return false;

    const std::string storedHash = std::string{ pwEl.get_string().value };

    const int rc = bcrypt_checkpw(password.c_str(), storedHash.c_str());
    if (rc == 0)  return true;

    return false;
}

string User::GetUserId(const string &email) {
    auto doc = DB::getOne("user", make_document(
        kvp("email", email)
        ));
    if (!doc) return nullptr;

    auto v = doc->view();
    return v["_id"].get_oid().value.to_string();
}

int User::usernameExist(string username) {
    if(!DB::exists("user", make_document(kvp("username", username)))){
        return 0;
    }
    return 1;
}

int User::emailExist(string email) {
    if(!DB::exists("user", make_document(kvp("email", email)))){
        return 0;
    }
    return 1;
}

bool User::ValidateRefreshToken(const string &token) {
    const string rtHash = hashToken(token);
    if(DB::exists("token", make_document(kvp("refreshToken", rtHash)))){
        return true;
    }
    return false;
}

bool User::DeleteRefreshToken(const string &token) {
    const string rtHash = hashToken(token);
    return DB::deleteOne("token", make_document(kvp("refreshToken", rtHash)));
}

bool User::DeleteAllRefreshToken(const string &userId) {
    return DB::deleteAll("token", make_document(kvp("userId", userId)));
}

bool User::InsertRefreshToken(string &token, const string &userId) {
    const string rtHash = hashToken(token);
    return DB::insert("token", make_document(
        kvp("userId", userId),
        kvp("refreshToken", rtHash)
        ), nullptr);
}

string User::GetIdFromRefreshToken(const string &token) {
    const string rtHash = hashToken(token);
    auto doc = DB::getOne("token", make_document(
        kvp("refreshToken", rtHash)
        ));
    auto v = doc->view();
    return v["userId"].get_oid().value.to_string();
}

std::string User::hashToken(const std::string &token) {
    auto toHex = [](const unsigned char* data, size_t len) -> std::string {
        std::ostringstream os;
        os << std::hex << std::setfill('0');
        for (size_t i = 0; i < len; ++i) os << std::setw(2) << static_cast<unsigned>(data[i]);
        return os.str();
    };

    const char* pepper = GetEnv("REFRESH_TOKEN_KEY").c_str();

    if (pepper && *pepper) {
        unsigned char mac[EVP_MAX_MD_SIZE];
        unsigned int macLen = 0;
        HMAC(EVP_sha256(),
             pepper, static_cast<int>(std::strlen(pepper)),
             reinterpret_cast<const unsigned char*>(token.data()), token.size(),
             mac, &macLen);
        return toHex(mac, macLen);                // recommended: store as rt_hash
    } else {
        unsigned char digest[SHA256_DIGEST_LENGTH];
        SHA256(reinterpret_cast<const unsigned char*>(token.data()), token.size(), digest);
        return toHex(digest, SHA256_DIGEST_LENGTH); // fallback if no pepper set
    }
}