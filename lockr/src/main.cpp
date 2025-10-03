#include "httplib.h"
#include "utils/config.h"
#include "utils/env.h"

#include "controller/api.h"
#include "db.h"
#include "nlohmann/json.hpp"

using nlohmann::json;
using namespace httplib;

int main() {
    Server svr;

    //init db
    InitDotEnv();
    Config::initialize();
    DB::connect();

    InitEndpoint(svr);

    svr.listen("0.0.0.0", 8080);
    return 0;
}