#include "controller/api/user.h"

void InitEndpoint(Server &svr) {
    // ----------------------
    //          USER
    // ----------------------
    svr.Post("/api/users/create", PostCreateUser);
}