#pragma once

#include <vector>

#include "ILogger.hpp"
#include "loggerBase.hpp"

namespace logger
{

class LoggerConf final
{
public:
    void add(LoggerBase logger);
    static LoggerConf& get();
    std::vector<LoggerBase>& getLoggers();

private:
    LoggerConf() = default;
    LoggerConf(const LoggerConf&) = delete;
    LoggerConf(const LoggerConf&&) = delete;
    LoggerConf(LoggerConf&) = delete;
    LoggerConf(LoggerConf&&) = delete;
    void operator=(LoggerConf&) = delete;

    std::vector<LoggerBase> loggers_;
};

} // namespace logger
