#pragma once

#include <mongocxx/database.hpp>
#include <mongocxx/instance.hpp>

namespace mongocxx { class client; }
using namespace std;

class DB {
public:
    static int insert(const string& coll,
                      const bsoncxx::document::view_or_value document,
                      string* out_err);

    static bool exists(const string& coll,
                       bsoncxx::document::view_or_value filter);

    static bool connect();


    static mongocxx::database& database() { return db_database; }
private:
    static unique_ptr<mongocxx::client> db_client;
    static mongocxx::database db_database;

    static void ensureTables();
};