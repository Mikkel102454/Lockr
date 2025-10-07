#pragma once

#include <string>

using namespace std;

bool ValidateAccessToken(const string &token);
int CreateNewAccessToken(const string &refreshToken, string &outToken);