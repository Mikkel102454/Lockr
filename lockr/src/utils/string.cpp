#include "utils/string.h"

vector<string> Split(const string& s, char delimiter) {
    vector<string> tokens;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delimiter)) {
        tokens.push_back(item);
    }
    return tokens;
}