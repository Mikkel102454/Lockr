#include "utils/user.h"

bool User::createUser(const std::string &username, const std::string &email, const std::string &password) {
    u_username = username;
    u_email = email;

    std::string hash;
    if (hashPassword(password, hash) != 0) {
        return false;
    }
    u_password = hash;

    return save();
}

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

bool User::save() {

}