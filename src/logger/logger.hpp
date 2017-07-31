#pragma once

#include <string>
#include <vector>

#include "ILogger.hpp"
#include "loggerBase.hpp"
#include "loggerConf.hpp"

namespace logger
{

class Logger
{
public:
    Logger(const std::string& name = "");
    ~Logger();
    void add(LoggerBase logger);

    template <typename T>
    Logger& operator<<(const T& data)
    {
        for (auto& logger : LoggerConf::get().getLoggers())
        {
            logger << data;
        }
        return *this;
    }

    Logger debug();
    Logger info();
    Logger warn();
    Logger err();

protected:
    std::string name_;
};

} // namespace logger
