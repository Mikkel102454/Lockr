#include "utils/config.h"
#include "utils/env.h"
#include "controller/api.h"
#include "db.h"

#include "httplib.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

int main() {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF);

    httplib::Server svr;

    //init db
    lockr::InitDotEnv();
    lockr::Config::Initialize();
    lockr::DB::Connect();

    lockr::InitEndpoint(svr);

    std::cout << "Webserver started!\n";
    svr.listen("0.0.0.0", 8080);
    return 0;
}