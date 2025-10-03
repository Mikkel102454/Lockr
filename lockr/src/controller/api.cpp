#include "controller/api/user.h"

void InitEndpoint(Server &svr) {
    // ----------------------
    //          USER
    // ----------------------
    svr.Post("/api/users/create", PostCreateUser);
    svr.Get("/api/users/username", GetUsernameValid);
    svr.Get("/api/users/email", GetEmailValid);
}