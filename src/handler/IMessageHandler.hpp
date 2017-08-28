#pragma once

#include <memory>

#include <ArduinoJson.h>

namespace handler
{

class IMessageHandler
{
public:
    using MessageHandlerPtr = std::unique_ptr<IMessageHandler>;

    virtual ~IMessageHandler() = default;

    virtual void onData(JsonObject& data) = 0;
};

} // namespace handler
