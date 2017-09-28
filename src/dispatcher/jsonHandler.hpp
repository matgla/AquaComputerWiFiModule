#pragma once

#include <map>
#include <string>
#include <vector>

#include <ArduinoJson.h>

#include "dispatcher/IHandler.hpp"
#include "dispatcher/messageHandler.hpp"
#include "logger/logger.hpp"
#include "utils/types.hpp"

namespace dispatcher
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

} // namespace dispatcher
