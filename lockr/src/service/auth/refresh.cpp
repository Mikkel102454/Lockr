#include "service/auth/refresh.h"
#include <iostream>
#include <random>
#include <iomanip>
#include <bsoncxx/builder/basic/document.hpp>

#include "db.h"
#include "class/user.h"

using namespace bsoncxx::builder::basic;

bool GenerateRefreshToken(std::string &outToken, const string &userId) {
    return User::InsertRefreshToken(outToken, userId);
}
bool InvalidateRefreshToken(const string &token) {
    return User::DeleteRefreshToken(token);
}

bool InvalidateUserRefreshToken(const string &userId) {
    return User::DeleteAllRefreshToken(userId);
}

bool ValidateRefreshToken(const string &token){
    return User::ValidateRefreshToken(token);
}

string GetIdFromRefreshToken(const string &token){
    return User::GetIdFromRefreshToken(token);
}