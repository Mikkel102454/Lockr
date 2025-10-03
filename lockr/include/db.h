#pragma once

#include <mongocxx/database.hpp>
#include <mongocxx/instance.hpp>

namespace mongocxx { class client; }

class DB {
public:
    static int insert(const std::string& coll,
                      const bsoncxx::document::view_or_value document,
                      std::string* out_err);

    static bool exists(const std::string& coll,
                       bsoncxx::document::view_or_value filter);

    static bool connect();


    static mongocxx::database& database() { return db_database; }
private:
    static std::unique_ptr<mongocxx::client> db_client;
    static mongocxx::database db_database;

    static void ensureTables();
};