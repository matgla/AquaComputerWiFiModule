#include "logger.hpp"

#include <mutex>

namespace logger
{

std::mutex logMutex;

Logger::Logger(const std::string& name)
    : name_(name)
{
}

Logger::~Logger()
{
    operator<<("\n");
    logMutex.unlock();
}

Logger Logger::debug()
{
    logMutex.lock();
    for (auto& logger : LoggerConf::get().getLoggers())
    {
        logger.debug(name_);
    }
    return Logger(name_);
}

Logger Logger::info()
{
    logMutex.lock();
    for (auto& logger : LoggerConf::get().getLoggers())
    {
        logger.info(name_);
    }
    return Logger(name_);
}

Logger Logger::warn()
{
    logMutex.lock();
    for (auto& logger : LoggerConf::get().getLoggers())
    {
        logger.warn(name_);
    }
    return Logger(name_);
}

Logger Logger::err()
{
    logMutex.lock();
    for (auto& logger : LoggerConf::get().getLoggers())
    {
        logger.err(name_);
    }
    return Logger(name_);
}

} // namespace logger
