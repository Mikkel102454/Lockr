#include "lockr/db.h"
#include "utils/config.h"
#include "utils/env.h"

#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/exception/exception.hpp>
#include <mongocxx/collection.hpp>
#include <mongocxx/options/insert.hpp>

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/document/view_or_value.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

std::unique_ptr<mongocxx::client> DB::db_client{};
mongocxx::database                 DB::db_database{};

bool DB::connect() {
    try {
        static mongocxx::instance inst{};

        auto uri = mongocxx::uri{GetEnv("DB_URI")};
        auto tmp = std::make_unique<mongocxx::client>(uri);

        db_database = (*tmp)[GetEnv("DB_NAME")];
        db_client = std::move(tmp);

        ensureTables();
        return true;
    } catch (const std::exception &e) {
        std::cerr << "DB connect error: " << e.what() << "\n";
        db_client.reset();
        return false;
    }
}


int DB::insert(const std::string& coll,
                  bsoncxx::document::view_or_value doc,
                  std::string* out_err) {
    try {
        auto c = db_database[coll];

        auto res = c.insert_one(doc);

        if (!res) {
            if (out_err) *out_err = "unacknowledged write";
            return 7;
        }
        return 0; // success
    }
    catch (const mongocxx::exception& e) {
        const std::string msg = e.what();
        if (out_err) *out_err = msg;

        // Errors
        if (msg.find("E11000") != std::string::npos)                return 1; // duplicate key
        if (msg.find("AuthenticationFailed") != std::string::npos
            || msg.find("auth") != std::string::npos)               return 2; // auth
        if (msg.find("No suitable servers") != std::string::npos
            || msg.find("socket") != std::string::npos
            || msg.find("connection") != std::string::npos)         return 3; // network/selection
        if (msg.find("timeout") != std::string::npos
            || msg.find("timed out") != std::string::npos)          return 4; // timeout
        if (msg.find("validation") != std::string::npos)            return 5; // schema/validator
        if (msg.find("WriteConcern") != std::string::npos
            || msg.find("wtimeout") != std::string::npos)           return 6; // write concern

        return 7; // other MongoDB error
    }
    catch (const std::exception& e) {
        if (out_err) *out_err = e.what();
        return 8;
    }
}


void DB::ensureTables() {

    // Users
    auto coll = db_database["users"];
    mongocxx::options::index uniq;
    uniq.unique(true);

    coll.create_index(make_document(kvp("username", 1)), uniq);
    coll.create_index(make_document(kvp("email", 1)), uniq);
    coll.create_index(make_document(kvp("companies", 1)));
}


bool DB::exists(const std::string& coll,
                bsoncxx::document::view_or_value filter) {
    auto c = db_database[coll];
    mongocxx::options::find opts;
    opts.limit(1);
    return static_cast<bool>(c.find_one(filter, opts));
}