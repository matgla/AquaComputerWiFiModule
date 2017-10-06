#pragma once

#include <memory>
#include <ostream>
#include <string>
#include <type_traits>

#include "logger/ILoggerBase.hpp"

namespace logger
{

class LoggerBase : public ILoggerBase
{
public:
    LoggerBase();
    virtual ~LoggerBase() = default;
    LoggerBase(const LoggerBase&) = default;
    LoggerBase(LoggerBase&&) = default;
    LoggerBase& operator=(const LoggerBase&&) = delete;
    LoggerBase& operator=(const LoggerBase&) = delete;

    template <typename T>
    LoggerBase& operator<<(const T& data)
    {
        (*stream_) << data; // NOLINT TODO: stadnik implement printer for different arrays
        return *this;
    }

    LoggerBase& debug(const std::string& componentName) override;
    LoggerBase& info(const std::string& componentName) override;
    LoggerBase& error(const std::string& componentName) override;
    LoggerBase& warn(const std::string& componentName) override;

protected:
    std::string getFormatedDateAndTime();

    std::shared_ptr<std::ostream> stream_;
};

} // namespace logger
