#pragma once

#include <vector>

#include "ILogger.hpp"
#include "loggerBase.hpp"

namespace logger
{

class Logger : public ILogger
{
public:
    void add(LoggerBase logger);

    template <typename T>
    Logger& operator<<(const T& data)
    {
        for (auto& logger : loggers_)
        {
            logger << data;
        }
        return *this;
    }

    Logger& info() override;
    Logger& warn() override;
    Logger& err() override;

protected:
    std::vector<LoggerBase> loggers_;
};

} // namespace logger
