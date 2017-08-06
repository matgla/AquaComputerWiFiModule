#include "logger.hpp"

#include <mutex>

namespace logger
{

static std::mutex logMutex;

Logger::Logger(const std::string& name, bool insertNewlineWhenDestruct)
    : name_(name),
      insertNewlineWhenDestruct_(insertNewlineWhenDestruct)
{
}

Logger::~Logger()
{
    if (insertNewlineWhenDestruct_)
    {
        operator<<("\n");
        logMutex.unlock();
    }
}

Logger Logger::debug()
{
    logMutex.lock();
    for (auto& logger : LoggerConf::get().getLoggers())
    {
        logger.debug(name_);
    }
    return Logger(name_, true);
}

Logger Logger::info()
{
    logMutex.lock();
    for (auto& logger : LoggerConf::get().getLoggers())
    {
        logger.info(name_);
    }
    return Logger(name_, true);
}

Logger Logger::warn()
{
    logMutex.lock();
    for (auto& logger : LoggerConf::get().getLoggers())
    {
        logger.warn(name_);
    }
    return Logger(name_, true);
}

Logger Logger::err()
{
    logMutex.lock();
    for (auto& logger : LoggerConf::get().getLoggers())
    {
        logger.err(name_);
    }
    return Logger(name_, true);
}

} // namespace logger
