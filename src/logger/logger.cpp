#include "logger.hpp"

namespace logger
{

void Logger::add(LoggerBase logger)
{
    loggers_.push_back(logger);
}

Logger& Logger::info()
{
    for (auto& logger : loggers_)
    {
        logger.info();
    }
    return *this;
}

Logger& Logger::warn()
{
    for (auto& logger : loggers_)
    {
        logger.warn();
    }
    return *this;
}

Logger& Logger::err()
{
    for (auto& logger : loggers_)
    {
        logger.err();
    }
    return *this;
}

}  // namespace logger
