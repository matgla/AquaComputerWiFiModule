#pragma once

#include <string>
#include <vector>

#include "ILogger.hpp"
#include "loggerBase.hpp"
#include "loggerConf.hpp"

namespace logger
{

class Logger : public ILogger
{
public:
    Logger(const std::string& name = "");
    void add(LoggerBase logger);

    template <typename T>
    Logger& operator<<(const T& data)
    {
        for (auto& logger : LoggerConf::get().getLoggers())
        {
            logger << data;
        }
        return *this;
    }

    Logger& info() override;
    Logger& warn() override;
    Logger& err() override;

protected:
    std::string name_;
};

} // namespace logger
