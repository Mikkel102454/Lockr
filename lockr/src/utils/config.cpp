#include "utils/config.h"

#include "utils/string.h"
#include "utils/exit.h"

void Config::initialize() {
    CONFIG = YAML::LoadFile("resources/config.yaml");
    if(!CONFIG) {
        exit(CONFIG_NOT_DEFINED);
    }
}

string Config::getString(const string& path){
    auto parts = Split(path, ':');
    YAML::Node current = YAML::Clone(CONFIG);
    for (const auto& p : parts) {
        if (!current[p]) {
            exit(CONFIG_ITEM_NOT_FOUND);
        }
        current = current[p];
    }
    return current.as<string>();
}

int Config::getInt(const string& path){
    auto parts = Split(path, ':');
    YAML::Node current = YAML::Clone(CONFIG);
    for (const auto& p : parts) {
        if (!current[p]) {
            exit(CONFIG_ITEM_NOT_FOUND);
        }
        current = current[p];
    }
    return current.as<int>();
}