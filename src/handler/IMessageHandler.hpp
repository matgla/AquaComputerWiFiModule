#pragma once

#include <memory>

class JsonObject;

namespace handler
{

class IMessageHandler
{
public:
    using MessageHandlerPtr = std::unique_ptr<IMessageHandler>;

    virtual ~IMessageHandler() = default;

    virtual void onData(const JsonObject& data) = 0;
};

} // namespace handler
