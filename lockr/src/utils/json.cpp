#include "utils/json.h"

#include <iostream>
namespace lockr {

    nlohmann::json GetJsonByPath(const nlohmann::json& root, const std::string& dot_path){
        const char* p = dot_path.c_str();
        const char* s = p;
        const char* e = p + dot_path.size();
        const nlohmann::json* cur = &root;
        std::cout << root;
        while (s < e) {
            const char* dot = static_cast<const char*>(memchr(s, '.', static_cast<size_t>(e - s)));
            std::string key = dot ? std::string(s, dot) : std::string(s, e);
            if (!cur->is_object() || !cur->contains(key)) return nullptr;
            cur = &(*cur)[key];
            if (!dot) break;
            s = dot + 1;
        }
        return *cur;
    }
}