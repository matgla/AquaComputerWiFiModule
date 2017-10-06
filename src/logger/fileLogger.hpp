#pragma once

#include <string>

#include "logger/loggerBase.hpp"

namespace logger
{
class FileLogger : public LoggerBase
{
public:
    FileLogger(const std::string& path);
    ~FileLogger() override = default;
    FileLogger(const FileLogger&) = default;
    FileLogger(FileLogger&&) = default;
    FileLogger& operator=(const FileLogger&&) = delete;
    FileLogger& operator=(const FileLogger&) = delete;
};
} // namespace logger
