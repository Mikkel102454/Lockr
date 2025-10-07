#include "utils/config.h"

#include "utils/string.h"
#include "utils/exit.h"

namespace lockr {
    void Config::Initialize() {
        CONFIG = YAML::LoadFile("resources/config.yaml");
        if(!CONFIG) {
            exit(CONFIG_NOT_DEFINED);
        }
    }

    std::string Config::GetString(const std::string& path){
        auto parts = Split(path, ':');
        YAML::Node current = YAML::Clone(CONFIG);
        for (const auto& p : parts) {
            if (!current[p]) {
                exit(CONFIG_ITEM_NOT_FOUND);
            }
            current = current[p];
        }
        return current.as<std::string>();
    }

    int Config::GetInt(const std::string& path){
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
}