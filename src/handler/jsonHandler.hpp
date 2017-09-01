#pragma once

#include <map>
#include <string>
#include <vector>

#include <ArduinoJson.h>

#include "handler/IHandler.hpp"
#include "handler/MessageHandler.hpp"
#include "logger/logger.hpp"
#include "utils/types.hpp"

namespace handler
{

class JsonHandler : public MessageHandler
{
public:
    JsonHandler();
    void handleData(const DataBuffer& data) override;
    void send(const JsonObject& message);

    void addMessageHandler(const std::string& name, IHandler::HandlerPtr handler);

protected:
    logger::Logger logger_;

    std::map<std::string, IHandler::HandlerPtr> handlers_;
};

} // namespace handler
