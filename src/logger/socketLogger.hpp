#pragma once

#include <string>

#include "loggerBase.hpp"
#include "utils/types.hpp"

namespace logger
{

class SocketLogger : public LoggerBase
{
public:
    SocketLogger() = delete;
    SocketLogger(const std::string& host, u16 port, const std::string& name = "default");
};

}  // namespace logger