#ifndef LOG_H
#define LOG_H

#include <string>

namespace lockr {
    enum LogLevel{
        INFO,
        WARNING,
        FATAL
    };

    void Log(LogLevel level, std::string message);
};


#endif //LOG_H
