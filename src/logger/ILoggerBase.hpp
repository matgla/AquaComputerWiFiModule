#pragma once

#include <string>

namespace logger
{

class ILoggerBase
{
public:
    virtual ILoggerBase& info(const std::string& componentName) = 0;
    virtual ILoggerBase& warn(const std::string& componentName) = 0;
    virtual ILoggerBase& err(const std::string& componentName) = 0;
};

} // namespace logger