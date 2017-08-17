#include "loggerBase.hpp"

#include <ctime>

namespace logger
{

LoggerBase::LoggerBase()
    : stream_(nullptr)
{
}

LoggerBase& LoggerBase::debug(const std::string& componentName)
{
    (*stream_) << "<" << getFormatedDateAndTime() << "> DBG/" << componentName << ": ";
    return *this;
}

LoggerBase& LoggerBase::info(const std::string& componentName)
{
    (*stream_) << "<" << getFormatedDateAndTime() << "> INF/" << componentName << ": ";
    return *this;
}

LoggerBase& LoggerBase::err(const std::string& componentName)
{
    (*stream_) << "<" << getFormatedDateAndTime() << "> ERR/" << componentName << ": ";
    return *this;
}

LoggerBase& LoggerBase::warn(const std::string& componentName)
{
    (*stream_) << "<" << getFormatedDateAndTime() << "> WRN/" << componentName << ": ";
    return *this;
}

std::string LoggerBase::getFormatedDateAndTime()
{
    const int TIME_BUFFER_SIZE = 18;
    char buffer[TIME_BUFFER_SIZE];
    auto t = std::time(nullptr);
    struct tm* currentTime = std::localtime(&t);
    std::strftime(buffer, TIME_BUFFER_SIZE, "%d/%m/%y %H:%M:%S", currentTime);
    return std::string(buffer);
}

} // namespace logger