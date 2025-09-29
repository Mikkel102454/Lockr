#pragma once
#include <memory>
#include <string>

namespace lockr {

    class Db {
    public:
        explicit Db(const std::string& uri);     // e.g. "mongodb://127.0.0.1:27017"
        ~Db();                                   // defined in .cpp

        // Example convenience API (expand as you need)
        bool ping();                              // returns true if server responds

        // If you really need raw collection access later, expose helpers with std::string,
        // not mongocxx types, to keep this header free of driver includes.

    private:
        struct Impl;                              // forward-declare the implementation
        std::unique_ptr<Impl> impl_;              // opaque pointer
    };

} // namespace lockr
