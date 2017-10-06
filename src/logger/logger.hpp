#pragma once

#include <string>
#include <type_traits>
#include <vector>

#include "ILogger.hpp"
#include "loggerBase.hpp"
#include "loggerConf.hpp"

namespace logger
{

class Logger
{
public:
    Logger(std::string name = "", bool insertNewlineWhenDestruct = false);
    Logger(const Logger&) = default;
    Logger(Logger&&) = default;
    Logger& operator=(const Logger&& other) = delete;
    Logger& operator=(const Logger& other) = delete;
    ~Logger();
    void add(LoggerBase logger);

    template <typename T>
    Logger& operator<<(const T& data)
    {
        for (auto& logger : LoggerConf::get().getLoggers())
        {
            logger << data; // NOLINT TODO: stadnik implement printer for different arrays
        }
        return *this;
    }

    Logger debug();
    Logger info();
    Logger warn();
    Logger error();

protected:
    std::string name_;
    bool insertNewlineWhenDestruct_;
};

} // namespace logger
