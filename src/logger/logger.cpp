#include "logger.hpp"

namespace logger
{

Logger::Logger(const std::string& name)
    : name_(name)
{
}

Logger& Logger::info()
{
    for (auto& logger : LoggerConf::get().getLoggers())
    {
        logger.info(name_);
    }
    return *this;
}

Logger& Logger::warn()
{
    for (auto& logger : LoggerConf::get().getLoggers())
    {
        logger.warn(name_);
    }
    return *this;
}

Logger& Logger::err()
{
    for (auto& logger : LoggerConf::get().getLoggers())
    {
        logger.err(name_);
    }
    return *this;
}

} // namespace logger
