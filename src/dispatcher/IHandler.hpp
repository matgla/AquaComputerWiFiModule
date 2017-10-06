#pragma once

#include <memory>

#include <ArduinoJson.h>

namespace dispatcher
{

class IHandler
{
public:
    using HandlerPtr = std::unique_ptr<IHandler>;
    IHandler() = default;
    IHandler(const IHandler&) = delete;
    IHandler(const IHandler&&) = delete;
    IHandler& operator=(const IHandler&& other) = delete;
    IHandler& operator=(const IHandler& other) = delete;
    virtual ~IHandler() = default;

    virtual void onData(const JsonObject& data) = 0;
};

} // namespace dispatcher
