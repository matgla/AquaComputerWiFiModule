#include "logger.hpp"

#include <mutex>

#ifndef X86_ARCH
namespace std
{
struct mutex
{
    void lock()
    {
    }
    void unlock()
    {
    }
};
}
#endif

namespace logger
{

static std::mutex logMutex;

Logger::Logger(std::string name, bool insertNewlineWhenDestruct)
    : name_(std::move(name)), insertNewlineWhenDestruct_(insertNewlineWhenDestruct)
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

Logger Logger::error()
{
    logMutex.lock();
    for (auto& logger : LoggerConf::get().getLoggers())
    {
        logger.error(name_);
    }
    return Logger(name_, true);
}

} // namespace logger
