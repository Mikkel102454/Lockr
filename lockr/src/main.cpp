#include "utils/config.h"
#include "utils/env.h"
#include "controller/api.h"
#include "db.h"

#include "httplib.h"

int main() {
    httplib::Server svr;

    //init db
    lockr::InitDotEnv();
    lockr::Config::Initialize();
    lockr::DB::Connect();

    lockr::InitEndpoint(svr);

    svr.listen("0.0.0.0", 8080);
    return 0;
}