#include "handler/jsonHandler.hpp"

#include <string>

#include <ArduinoJson.h>

namespace handler
{

JsonHandler::JsonHandler() : logger_("JsonHandler")
{
}

void JsonHandler::onRead(std::vector<u8> data)
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

    // handlers_[key]->onData(obj);
}

// void JsonHandler::addMessageHandler(const std::string& messageName,
//                                     IMessageHandler::MessageHandlerPtr handler)
// {
//     if (handlers_.count(messageName) != 0)
//     {
//         logger_.warn() << "Handler for message " << messageName
//                        << " exists and will be replaced with new one";
//     }
//     handlers_[messageName] = std::move(handler);
// }


} // namespace handler
