#include "handler/handshakeHandler.hpp"

namespace handler
{

HandshakeHandler::HandshakeHandler(handler::JsonHandler* handler)
    : handler_(handler), logger_("HandshakeHandler")
{
}


void HandshakeHandler::onData(const JsonObject& data)
{
    logger_.debug() << "Received handshake from MCU";
    DynamicJsonBuffer buffer;

    auto& moduleInfo = buffer.createObject();
    moduleInfo["id"] = "module_info";

    if (handler_)
    {
        handler_->send(moduleInfo);
        logger_.debug() << "Module info to MCU was sent";
    }
    else
    {
        logger_.debug() << "Handler not exist, can't send module info";
    }
}
} // namespace handler
