#include "loggerConf.hpp"

namespace logger
{

void LoggerConf::add(LoggerBase logger)
{
    loggers_.push_back(logger);
}


LoggerConf& LoggerConf::get()
{
    static LoggerConf instance;
    return instance;
}

std::vector<LoggerBase>& LoggerConf::getLoggers()
{
    return loggers_;
}

} // namespace logger
