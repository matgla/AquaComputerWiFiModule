#pragma once

#include <memory>
#include <ostream>
#include <string>

#include "logger/ILogger.hpp"

namespace logger
{

class LoggerBase : public ILogger
{
public:
    LoggerBase();
    explicit LoggerBase(const std::string& name);
    virtual ~LoggerBase() = default;

    template <typename T>
    LoggerBase& operator<<(const T& data)
    {
        (*stream_) << data;
        return *this;
    }

    LoggerBase& info() override;
    LoggerBase& err() override;
    LoggerBase& warn() override;

protected:
    std::string getFormatedDateAndTime();

    std::shared_ptr<std::ostream> stream_;
    std::string name_;
};

} // namespace logger