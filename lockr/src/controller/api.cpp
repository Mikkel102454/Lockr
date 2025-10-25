#include "controller/api/company.h"
#include "controller/api/user.h"
#include "controller/api/token.h"
#include "controller/api/frontend/page.h"
#include "controller/api/frontend/resource.h"

namespace lockr {
    void InitEndpoint(httplib::Server &svr) {
        // ----------------------
        //        FRONTEND
        // ----------------------
        svr.Get(R"(/style/([^/]+)?)", GetStyle);
        svr.Get(R"(/code/([^/]+)?)", GetCode);
        svr.Get("/", GetRootPage);
        svr.Get(R"(/([^/]+)?)", GetPage);

        // ----------------------
        //          USER
        // ----------------------

        // Username - string
        // Email - string
        // Password - string
        svr.Post("/api/users/create", PostCreateUser);

        // Email - string
        // Password - string
        // Company Id - string
        svr.Post("/api/users/login", PostLogin);

        // Username - string
        svr.Post("/api/users/username", PostUsernameValid);

        // Email - string
        svr.Post("/api/users/email", PostEmailValid);

        // ----------------------
        //         TOKEN
        // ----------------------

        // Refresh Token - string
        // Company Id - string
        svr.Post("/api/token/refresh", PostRefreshToken);

        // Refresh Token - string
        // Company Id - string
        svr.Post("/api/temp/token/refresh", PostValidateRefreshToken);

        // Access Token - string
        // Company Id - string
        svr.Post("/api/temp/token/access", PostValidateAccessToken);

        // ----------------------
        //        Company
        // ----------------------

        // Company Token - string
        // User Id - string
        // Json - string
        // Replace - bool
        svr.Put("/api/company/data", PutUserData);

        // Company Token - string
        // User Id - string
        // Fields (optional) - string
        svr.Post("/api/company/data/get", PostGetUserData);

        // Access Token - string
        // Company Name - string
        svr.Post("/api/company/key/create", PostCreateKey);
    }
}
