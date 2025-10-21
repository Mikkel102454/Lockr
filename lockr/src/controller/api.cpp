#include "controller/api/company.h"
#include "controller/api/user.h"
#include "controller/api/token.h"
#include "service/company/api/create.h"

namespace lockr {
    void InitEndpoint(httplib::Server &svr) {
        // ----------------------
        //          USER
        // ----------------------
        svr.Post("/api/users/create", PostCreateUser);
        svr.Post("/api/users/login", PostLogin);
        svr.Post("/api/users/username", PostUsernameValid);
        svr.Post("/api/users/email", PostEmailValid);

        // ----------------------
        //         TOKEN
        // ----------------------
        svr.Post("/api/token/refresh", PostRefreshToken);

        svr.Post("/api/temp/token/refresh", PostValidateRefreshToken);
        svr.Post("/api/temp/token/access", PostValidateAccessToken);

        // ----------------------
        //        Company
        // ----------------------
        svr.Put("/api/company/data", PutUserData);
        svr.Get("/api/company/data", GetUserData);

        svr.Post("/api/company/key/create", PostCreateKey);
    }
}
