#pragma once
#include <yaml-cpp/yaml.h>

using namespace std;

class Config {
public:
    static string getString(const string &path);
    static int getInt(const string &path);
    static void initialize();
private:
    inline static YAML::Node CONFIG{};
    inline static YAML::Node ENV{};
};