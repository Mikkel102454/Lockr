#include <iostream>
#include "utils/configHandler.h"
#include "utils/string.h"

static const YAML::Node CONFIG = YAML::LoadFile("resources/config.yaml");

template<typename T>
T GetConfig(const std::string& path) {
    auto parts = split(path, ':');

    YAML::Node current = YAML::Clone(CONFIG);
    for (const auto& p : parts) {
        if (!current[p]) {
            std::exit(1);
        }
        current = current[p];
    }

    return current.as<T>();
}

template std::string GetConfig<std::string>(const std::string& path);
template int         GetConfig<int>(const std::string& path);