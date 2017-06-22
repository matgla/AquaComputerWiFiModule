#pragma once

#include <iostream>
#include <string>

#include "loggerBase.hpp"

namespace logger
{

class StdOutLogger : public LoggerBase
{
public:
    StdOutLogger();
    StdOutLogger(const std::string& name);
};

}  // namespace logger