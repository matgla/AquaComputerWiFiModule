#pragma once

#include <vector>

#include "ILogger.hpp"
#include "loggerBase.hpp"

namespace logger
{

class LoggerConf final
{
public:
    LoggerConf(const LoggerConf&) = delete;
    LoggerConf(const LoggerConf&&) = delete;
    LoggerConf& operator=(const LoggerConf&) = delete;
    LoggerConf& operator=(const LoggerConf&&) = delete;
    ~LoggerConf() = default;

    void add(const LoggerBase& logger);
    static LoggerConf& get();
    std::vector<LoggerBase>& getLoggers();

private:
    LoggerConf() = default;

    std::vector<LoggerBase> loggers_;
};

} // namespace logger
