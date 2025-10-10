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

namespace lockr {
    std::unique_ptr<mongocxx::client> DB::mClient{};
    mongocxx::database                 DB::mDatabase{};

    bool DB::Connect() {
        try {
            static mongocxx::instance inst{};

            std::string uriString =
                    "mongodb://" + GetEnv("DB_USER") + ":" + GetEnv("DB_PASS") +
                    "@" + GetEnv("DB_IP") + "/" + GetEnv("DB_NAME") +
                    "?authSource=" + GetEnv("DB_NAME");

            auto uri = mongocxx::uri{uriString};
            auto tmp = std::make_unique<mongocxx::client>(uri);

            mDatabase = (*tmp)[GetEnv("DB_NAME")];
            mClient = std::move(tmp);

            EnsureTables();
            return true;
        } catch (const std::exception &e) {
            std::cerr << "DB connect error: " << e.what() << "\n";
            mClient.reset();
            return false;
        }
    }


    int DB::Insert(const std::string& coll,
                   bsoncxx::document::view_or_value document,
                   std::string* out_err) {
        try {
            auto c = mDatabase[coll];

            auto res = c.insert_one(std::move(document));

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


    void DB::EnsureTables() {

        // user
        auto coll = mDatabase["user"];
        mongocxx::options::index uniq;
        uniq.unique(true);

        coll.create_index(bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("username", 1)), uniq);

        coll.create_index(bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("email", 1)), uniq);

        coll.create_index(bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("companies", 1)));
    }

    std::optional<bsoncxx::document::value> DB::getOne(const std::string& coll,
                                                       bsoncxx::document::view_or_value filter) {
        auto c = mDatabase[coll];
        mongocxx::options::find opts;
        opts.limit(1);
        if (auto doc = c.find_one(std::move(filter), opts)) {
            return std::move(*doc);
        }
        return std::nullopt;
    }

    bool DB::Exists(const std::string& coll,
                    bsoncxx::document::view_or_value filter) {
        auto c = mDatabase[coll];
        mongocxx::options::find opts;
        opts.limit(1);
        return static_cast<bool>(c.find_one(std::move(filter), opts));
    }

    bool DB::DeleteOne(const std::string& coll,
                       bsoncxx::document::view_or_value filter) {
        auto c = mDatabase[coll];
        auto res = c.delete_one(std::move(filter));
        return res && res->deleted_count() == 1;
    }

    bool DB::DeleteAll(const std::string& coll,
                       bsoncxx::document::view_or_value filter) {
        auto c = mDatabase[coll];
        auto res = c.delete_many(std::move(filter));
        return res && res->deleted_count() > 0;
    }

    bool DB::ReplaceOne(const std::string& coll,
                       bsoncxx::document::view_or_value filter,
                       bsoncxx::document::view_or_value data) {

    }
}