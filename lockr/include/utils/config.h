#ifndef LOCKR_UTILS_CONFIG
#define LOCKR_UTILS_CONFIG 1

#include <yaml-cpp/yaml.h>

namespace lockr {
    class Config {
    public:
        static std::string GetString(const std::string &path);
        static int GetInt(const std::string &path);
        static void Initialize();
    private:
        inline static YAML::Node CONFIG{};
        inline static YAML::Node ENV{};
    };
}


#endif