#pragma once

#include <string>

#include "logger/loggerBase.hpp"

namespace logger
{
class FileLogger : public LoggerBase
{
public:
    FileLogger(const std::string& path);
};
} // namespace logger
