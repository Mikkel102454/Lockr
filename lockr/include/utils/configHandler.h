#pragma once
#include "yaml-cpp/yaml.h"

template<typename T>
inline T GetConfig(const std::string& path);