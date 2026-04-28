#include "utils/config.h"
#include "utils/env.h"
#include "controller/api.h"
#include "db.h"

#include "httplib.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

int setup();

int main() {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF);

    httplib::Server svr;

    //init db
    lockr::InitDotEnv();
    lockr::Config::Initialize();
    lockr::DB::Connect();

    setup();

    lockr::InitEndpoint(svr);

    std::cout << "Webserver started!\n";
    svr.listen(lockr::Config::GetString("server:ip"), lockr::Config::GetInt("server:port"));
    return 0;
}