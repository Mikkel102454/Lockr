#ifndef LOCKR_DB
#define LOCKR_DB 1

#include <mongocxx/database.hpp>
#include <mongocxx/instance.hpp>

namespace mongocxx { class client; }
namespace lockr {
    class DB {
    public:
        static int Insert(const std::string& coll,
                          bsoncxx::document::view_or_value document,
                          std::string* out_err);

        static std::string Insert(const std::string& coll,
               bsoncxx::document::view_or_value document);

        static std::optional<bsoncxx::document::value> getOne(const std::string& coll,
                                                         bsoncxx::document::view_or_value filter);

        static bool Exists(const std::string& coll,
                           bsoncxx::document::view_or_value filter);

        static bool DeleteOne(const std::string& coll,
                              bsoncxx::document::view_or_value filter);

        static bool DeleteAll(const std::string& coll,
                              bsoncxx::document::view_or_value filter);

        static bool UpdateOne(const std::string& coll,
                       bsoncxx::document::view_or_value filter,
                       bsoncxx::document::view_or_value data);

        static bool Connect();


        static mongocxx::database& Database() { return mDatabase; }
    private:
        static std::unique_ptr<mongocxx::client> mClient;
        static mongocxx::database mDatabase;

        static void EnsureTables();
    };
}

#endif