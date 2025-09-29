#include "lockr/db.hpp"

#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <bsoncxx/builder/basic/document.hpp>

namespace {

// one driver instance for the whole process
    mongocxx::instance& driver()
    {
        static mongocxx::instance inst{};
        return inst;
    }

} // namespace

namespace lockr {

    struct Db::Impl {
        explicit Impl(const std::string& uri)
                : client{ mongocxx::uri{ uri } } {}

        mongocxx::client client;
    };

    Db::Db(const std::string& uri)
            : impl_{ std::make_unique<Impl>(uri) }
    {
        (void)driver(); // ensure driver constructed before using the client
    }

    Db::~Db() = default;

    bool Db::ping()
    {
        auto admin = impl_->client["admin"];
        bsoncxx::builder::basic::document cmd;
        cmd.append(bsoncxx::builder::basic::kvp("ping", 1));
        auto res = admin.run_command(cmd.view());
        return res.view().find("ok") != res.view().end();
    }

} // namespace lockr
