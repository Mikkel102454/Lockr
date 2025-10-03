#include "utils/env.h"

#include "utils/exit.h"

#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#endif

static inline string trim(string s) {
    auto issp = [](unsigned char c){ return isspace(c); };
    s.erase(s.begin(), find_if(s.begin(), s.end(), [&](unsigned char c){ return !issp(c); }));
    s.erase(find_if(s.rbegin(), s.rend(), [&](unsigned char c){ return !issp(c); }).base(), s.end());
    return s;
}

void InitDotEnv() {
    filesystem::path exeDir;
#ifdef _WIN32
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    exeDir = filesystem::path(exePath).parent_path();
#else
    exeDir = filesystem::current_path();
#endif

    ifstream f(exeDir / ".env");
    if (!f) return;

    string line;
    while (getline(f, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;

        auto pos = line.find('=');
        if (pos == string::npos) continue;

        string key = trim(line.substr(0, pos));
        string val = trim(line.substr(pos + 1));

#ifdef _WIN32
        _putenv_s(key.c_str(), val.c_str());
#else
        // putenv requires storage to persist for the program lifetime
        static vector<string> storage;
        storage.push_back(key + "=" + val);
        putenv(storage.back().data());
#endif
    }
}

string GetEnv(const string& key) {
#ifdef _WIN32
    size_t len = 0;
    getenv_s(&len, nullptr, 0, key.c_str());
    if (len == 0) exit(ENV_VARIABLE_NOT_FOUND);

    string value(len, '\0');
    getenv_s(&len, &value[0], len, key.c_str());
    if (!value.empty() && value.back() == '\0') value.pop_back(); // remove null terminator
    return value;
#else
    const char* val = getenv(key.c_str());
    return val ? string(val) : exit(ENV_VARIABLE_NOT_FOUND);
#endif
}