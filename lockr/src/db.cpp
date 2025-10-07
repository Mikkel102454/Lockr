#include "db.h"
#include "utils/config.h"
#include "utils/env.h"

#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/exception/exception.hpp>
#include <mongocxx/collection.hpp>

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/document/view_or_value.hpp>
#include <utility>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

unique_ptr<mongocxx::client> DB::db_client{};
mongocxx::database                 DB::db_database{};

bool DB::connect() {
    try {
        static mongocxx::instance inst{};

        string uriString =
            "mongodb://" + GetEnv("DB_USER") + ":" + GetEnv("DB_PASS") +
            "@" + GetEnv("DB_IP") + "/" + GetEnv("DB_NAME") +
            "?authSource=" + GetEnv("DB_NAME");

        auto uri = mongocxx::uri{uriString};
        auto tmp = make_unique<mongocxx::client>(uri);

        db_database = (*tmp)[GetEnv("DB_NAME")];
        db_client = move(tmp);

        ensureTables();
        return true;
    } catch (const exception &e) {
        cerr << "DB connect error: " << e.what() << "\n";
        db_client.reset();
        return false;
    }
}


int DB::insert(const string& coll,
                  bsoncxx::document::view_or_value document,
                  string* out_err) {
    try {
        auto c = db_database[coll];

        auto res = c.insert_one(move(document));

        if (!res) {
            if (out_err) *out_err = "unacknowledged write";
            return 7;
        }
        return 0; // success
    }
    catch (const mongocxx::exception& e) {
        const string msg = e.what();
        if (out_err) *out_err = msg;

        // Errors
        if (msg.find("E11000") != string::npos)                return 1; // duplicate key
        if (msg.find("AuthenticationFailed") != string::npos
            || msg.find("auth") != string::npos)               return 2; // auth
        if (msg.find("No suitable servers") != string::npos
            || msg.find("socket") != string::npos
            || msg.find("connection") != string::npos)         return 3; // network/selection
        if (msg.find("timeout") != string::npos
            || msg.find("timed out") != string::npos)          return 4; // timeout
        if (msg.find("validation") != string::npos)            return 5; // schema/validator
        if (msg.find("WriteConcern") != string::npos
            || msg.find("wtimeout") != string::npos)           return 6; // write concern

        return 7; // other MongoDB error
    }
    catch (const exception& e) {
        if (out_err) *out_err = e.what();
        return 8;
    }
}


void DB::ensureTables() {

    // user
    auto coll = db_database["user"];
    mongocxx::options::index uniq;
    uniq.unique(true);

    coll.create_index(make_document(kvp("username", 1)), uniq);
    coll.create_index(make_document(kvp("email", 1)), uniq);
    coll.create_index(make_document(kvp("companies", 1)));
}

optional<bsoncxx::document::value> DB::getOne(const string& coll,
                bsoncxx::document::view_or_value filter) {
    auto c = db_database[coll];
    mongocxx::options::find opts;
    opts.limit(1);
    if (auto doc = c.find_one(std::move(filter), opts)) {
        return std::move(*doc);
    }
    return std::nullopt;
}

bool DB::exists(const string& coll,
                bsoncxx::document::view_or_value filter) {
    auto c = db_database[coll];
    mongocxx::options::find opts;
    opts.limit(1);
    return static_cast<bool>(c.find_one(move(filter), opts));
}

bool DB::deleteOne(const std::string& coll,
                   bsoncxx::document::view_or_value filter) {
    auto c = db_database[coll];
    auto res = c.delete_one(std::move(filter));
    return res && res->deleted_count() == 1;
}

bool DB::deleteAll(const std::string& coll,
                   bsoncxx::document::view_or_value filter) {
    auto c = db_database[coll];
    auto res = c.delete_many(std::move(filter));
    return res && res->deleted_count() > 0;
}