#include "utils/env.h"

#include "utils/exit.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <cctype>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#endif

static inline std::string trim(std::string s) {
    auto issp = [](unsigned char c){ return std::isspace(c); };
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [&](unsigned char c){ return !issp(c); }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [&](unsigned char c){ return !issp(c); }).base(), s.end());
    return s;
}

void InitDotEnv() {
    std::filesystem::path exeDir;
#ifdef _WIN32
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    exeDir = std::filesystem::path(exePath).parent_path();
#else
    exeDir = std::filesystem::current_path();
#endif

    std::ifstream f(exeDir / ".env");
    if (!f) return;

    std::string line;
    while (std::getline(f, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;

        auto pos = line.find('=');
        if (pos == std::string::npos) continue;

        std::string key = trim(line.substr(0, pos));
        std::string val = trim(line.substr(pos + 1));

#ifdef _WIN32
        _putenv_s(key.c_str(), val.c_str());
#else
        // putenv requires storage to persist for the program lifetime
        static std::vector<std::string> storage;
        storage.push_back(key + "=" + val);
        putenv(storage.back().data());
#endif
    }
}

std::string GetEnv(const std::string& key) {
#ifdef _WIN32
    size_t len = 0;
    getenv_s(&len, nullptr, 0, key.c_str());
    if (len == 0) std::exit(ENV_VARIABLE_NOT_FOUND);

    std::string value(len, '\0');
    getenv_s(&len, &value[0], len, key.c_str());
    if (!value.empty() && value.back() == '\0') value.pop_back(); // remove null terminator
    return value;
#else
    const char* val = std::getenv(key.c_str());
    return val ? std::string(val) : std::exit(ENV_VARIABLE_NOT_FOUND);
#endif
}