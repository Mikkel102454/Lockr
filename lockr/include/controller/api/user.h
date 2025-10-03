#pragma once

#include "controller/api.h"

void PostCreateUser(const Request& req, Response& res);
void GetUsernameValid(const Request& req, Response& res);
void GetEmailValid(const Request& req, Response& res);