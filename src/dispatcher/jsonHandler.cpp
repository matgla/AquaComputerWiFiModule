#include "dispatcher/jsonHandler.hpp"

#include <string>

#include <ArduinoJson.h>

namespace dispatcher
{

JsonHandler::JsonHandler() : logger_("JsonHandler")
{
}

void JsonHandler::handleData(const DataBuffer& data)
{
    DynamicJsonBuffer buffer;
    JsonObject& obj = buffer.parseObject(data.data());

    if (!obj.containsKey("id"))
    {
        logger_.error() << "Received wrong JSON message...";
        return;
    }

    const char* key = obj["id"].as<const char*>();

    if (0 == handlers_.count(key))
    {
        logger_.warn() << "Can't find handler for message: " << key;
        return;
    }

    handlers_[key]->onData(obj);
}

void JsonHandler::addMessageHandler(const std::string& name, IHandler::HandlerPtr handler)
{
    if (handlers_.count(name) != 0)
    {
        logger_.warn() << "Handler for message " << name
                       << " exists and will be replaced with new one";
    }
    handlers_[name] = std::move(handler);
}

void JsonHandler::send(const JsonObject& message)
{
    std::string buffer;
    message.printTo(buffer);

    MessageHandler::send(buffer);
}

} // namespace dispatcher
