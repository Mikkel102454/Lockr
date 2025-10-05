#include "service/auth/refresh.h"
#include <iostream>
#include <random>
#include <iomanip>

bool GenerateRefreshToken(std::string &outToken) {
    return true;
}
bool InvalidateRefreshToken(const string &token) {
    return true;
}

bool InvalidateUserRefreshToken(const string &userUuid) {
    return true;
}

bool ValidateRefreshToken(string outToken){
    GenerateRefreshToken(outToken);

    return true;
}