#include "service/user/login.h"

#include "class/user.h"
#include "service/auth/refresh.h"

bool Login(const string &email, const string &password, string &outToken) {
    if (!User::Authenticate(email, password)) {
        return false;
    }


    GenerateRefreshToken(outToken, User::GetUserId(email));
    return true;
}
