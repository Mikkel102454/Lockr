#pragma once

#include "controller/api.h"

void PostRefreshToken(const Request& req, Response& res);


void PostValidateRefreshToken(const Request& req, Response& res);
void PostValidateAccessToken(const Request& req, Response& res);