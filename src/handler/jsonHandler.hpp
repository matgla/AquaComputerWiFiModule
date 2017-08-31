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
    void onRead(DataBuffer data);
    void send(const JsonObject& message);

protected:
    logger::Logger logger_;

    std::map<std::string, IHandler::HandlerPtr> handlers_;
};

} // namespace handler
