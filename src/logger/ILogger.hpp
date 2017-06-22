#pragma once

namespace logger
{

class ILogger
{
public:
    virtual ILogger& info() = 0;
    virtual ILogger& warn() = 0;
    virtual ILogger& err() = 0;
};

}  // namespace logger