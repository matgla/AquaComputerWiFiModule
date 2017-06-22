#include "loggerBase.hpp"

#include <ctime>

namespace logger
{

LoggerBase::LoggerBase() :
    stream_(nullptr),
    name_("default")
{}

LoggerBase::LoggerBase(const std::string& name) :
    stream_(nullptr),
    name_(name)
{}

LoggerBase& LoggerBase::info()
{
    (*stream_) << "<" << getFormatedDateAndTime() << "> INF/" << name_ << ": ";
    return *this;
}

LoggerBase& LoggerBase::err()
{
    (*stream_) << "<" << getFormatedDateAndTime() << "> ERR/" << name_ << ": ";
    return *this;
}

LoggerBase& LoggerBase::warn()
{
    (*stream_) << "<" << getFormatedDateAndTime() << "> WRN/" << name_ << ": ";
    return *this;
}

std::string LoggerBase::getFormatedDateAndTime()
{
    const int TIME_BUFFER_SIZE = 18;
    char buffer[TIME_BUFFER_SIZE];
    auto t = std::time(nullptr);
    struct tm *currentTime = std::localtime(&t);
    std::strftime(buffer, TIME_BUFFER_SIZE, "%d/%m/%y %H:%M:%S", currentTime);
    return std::string(buffer);
}

}  // namespace logger