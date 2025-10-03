#include "controller/api/user.h"

void initEndpoint(Server &svr) {
    // ----------------------
    //          USER
    // ----------------------
    svr.Post("/api/users/create", PostCreateUser);
}