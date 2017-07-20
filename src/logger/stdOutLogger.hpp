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
};

} // namespace logger