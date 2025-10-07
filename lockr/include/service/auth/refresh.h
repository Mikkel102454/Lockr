#pragma once

#include <string>

using namespace std;

bool GenerateRefreshToken(std::string &outToken, const string &userId);
bool InvalidateRefreshToken(const string &token);
bool InvalidateUserRefreshToken(const string &userUuid);
bool ValidateRefreshToken(const string &token);
string GetIdFromRefreshToken(const string &token);