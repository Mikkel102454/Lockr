#include <iostream>
#include "utils/config.h"
#include "utils/string.h"
#include "utils/exit.h"

void Config::initialize() {
    CONFIG = YAML::LoadFile("resources/config.yaml");
    if(!CONFIG) {
        std::exit(CONFIG_NOT_DEFINED);
    }
}

std::string Config::getString(const std::string& path){
    auto parts = split(path, ':');
    YAML::Node current = YAML::Clone(CONFIG);
    for (const auto& p : parts) {
        if (!current[p]) {
            std::exit(CONFIG_ITEM_NOT_FOUND);
        }
        current = current[p];
    }
    return current.as<std::string>();
}

int Config::getInt(const std::string& path){
    auto parts = split(path, ':');
    YAML::Node current = YAML::Clone(CONFIG);
    for (const auto& p : parts) {
        if (!current[p]) {
            std::exit(CONFIG_ITEM_NOT_FOUND);
        }
        current = current[p];
    }
    return current.as<int>();
}