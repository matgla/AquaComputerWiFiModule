#pragma once

#include <iostream>
#include <string>

#include "loggerBase.hpp"

namespace logger
{

class StdErrLogger : public LoggerBase
{
public:
    StdErrLogger();
};

} // namespace logger
