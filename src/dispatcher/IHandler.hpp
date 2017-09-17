#pragma once

#include <memory>

#include <ArduinoJson.h>

namespace dispatcher
{

class IHandler
{
public:
    using HandlerPtr = std::unique_ptr<IHandler>;

    virtual ~IHandler() = default;

    virtual void onData(const JsonObject& data) = 0;
};

} // namespace dispatcher
