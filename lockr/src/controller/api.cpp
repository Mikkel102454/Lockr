#include "controller/api/user.h"
#include "controller/api/token.h"

namespace lockr {
    void InitEndpoint(httplib::Server &svr) {
        // ----------------------
        //          USER
        // ----------------------
        svr.Post("/api/users/create", PostCreateUser);
        svr.Post("/api/users/login", PostLogin);
        svr.Get("/api/users/username", GetUsernameValid);
        svr.Get("/api/users/email", GetEmailValid);

        // ----------------------
        //         TOKEN
        // ----------------------
        svr.Post("/api/token/refresh", PostRefreshToken);

        svr.Post("/api/temp/token/refresh", PostValidateRefreshToken);
        svr.Post("/api/temp/token/access", PostValidateAccessToken);

    }
}