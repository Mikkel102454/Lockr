#pragma once
#include <yaml-cpp/yaml.h>

class Config {
public:
    static std::string getString(const std::string &path);
    static int getInt(const std::string &path);
    static void initialize();
private:
    inline static YAML::Node CONFIG{};
    inline static YAML::Node ENV{};
};