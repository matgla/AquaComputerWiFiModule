#include "statemachine/mcuConnection.hpp"

#include <string>

namespace statemachine
{

void McuConnection::sendHandshake(HandlerPtr handler, logger::Logger& logger)
{
    std::string msg = "{\"id\":\"handshake\"}";
    DynamicJsonBuffer buffer;
    JsonObject& jsonMsg = buffer.parseObject(msg);
    handler->send(jsonMsg);
    logger.info() << "Handshake was sent to MCU";
}

void McuConnection::processConnected(HandlerPtr handler, logger::Logger& logger)
{
    logger.info() << "Connected to MCU!";
}

} // namespace statemachine
