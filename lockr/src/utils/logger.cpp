#include "utils/logger.h"

#include <iostream>

namespace lockr {
    void Log(LogLevel level, const std::string message) {
        std::cout << message << "\n";
    }
}